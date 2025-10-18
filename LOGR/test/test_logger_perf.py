import argparse
import re
import statistics
import sys

from dataclasses import dataclass
from itertools import zip_longest
from pathlib import Path

import socket_testing

WARMUP_REPETITIONS = 1
REPETITIONS = 5

MIN_THRESHOLD_NS = 25
MAX_THRESHOLD_NS = 125

BASELINE_FILE = "LOGR_perf_baseline.txt"
TEST_FILE = "LOGR_perf_test.txt"
COMMENT_FILE = "LOGR_perf_comment.md"

@dataclass
class Results:
    times: list[int]
    mean: float
    stddev: float

    def write(self, path: Path | str):
        with open(path, "w") as f:
            for idx, time in enumerate(self.times):
                f.write(f"Run {idx + 1}: {time:.0f}\n")
            f.write(f"Mean: {self.mean:.0f}\n")
            f.write(f"Stddev: {self.stddev:.0f}\n")
        print("Wrote", str(path))

class Validation:
    def __init__(self, baseline: Results, test: Results):
        clamp = lambda min_val, val, max_val: min(max(min_val, val), max_val)

        self.baseline = baseline
        self.test = test
        self.diff = test.mean - baseline.mean
        self.threshold = clamp(MIN_THRESHOLD_NS, 2 * baseline.stddev, MAX_THRESHOLD_NS)
        self.regression = self.diff > self.threshold

def parse_results(path: Path | str):
    with open(path, "r") as f:
        content = f.read()
        times = [int(t) for t in re.findall(r"Run \d+: (\d+)", content)]
        if not times:
            raise RuntimeError("No runs found in results file")

        mean = re.search(r"Mean: (\d+)", content)
        stddev = re.search(r"Stddev: (\d+)", content)
        if not mean or not stddev:
            raise RuntimeError("Failed to parse results file")

        mean = float(mean.group(1))
        stddev = float(stddev.group(1))

    return Results(times, mean, stddev)

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("exec", help="performance test executable")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--baseline", action="store_true",
                       help="generate a test baseline to compare to")
    group.add_argument("--test", action="store_true",
                       help="test against the existing baseline")
    args = parser.parse_args()
    return args

def cleanup_logs():
    for path in Path('.').glob('LOGR_PERF*csv'):
        path.unlink()

def run_single(exec_path: str) -> int:
    proc = socket_testing.launch_sut(exec_path, text=True)

    out, _ = proc.communicate(timeout=5)

    if proc.returncode != 0:
        raise RuntimeError(f"Performance test failed with code {proc.returncode}")

    match = re.search(r"Average time per log: (\d+)ns", out)
    if not match:
        raise RuntimeError(f"Failed to parse performance test output:\n{out}")

    return int(match.group(1))

def run(exec_path: str, repetitions: int) -> Results:
    assert repetitions > 0

    times = [run_single(exec_path) for _ in range(repetitions)]
    return Results(times, statistics.mean(times), statistics.stdev(times) if len(times) > 1 else 0.0)

def format_into_markdown_table(validation: Validation) -> str:
    mean_diff = validation.test.mean - validation.baseline.mean
    stddev_diff = validation.test.stddev - validation.baseline.stddev
    regression = "FAIL" if validation.regression else "OK"

    class FormattedRun:
        def __init__(self, idx: int, baseline: float | None, test: float | None):
            spaces = lambda n: ' ' * n

            self.number = f"{idx + 1:>4}"
            self.baseline = f"{baseline:13.0f}" if baseline else "-" + spaces(12)
            self.test = f"{test:9.0f}" if test else "-" + spaces(8)
            self.diff = (
                f"{test - baseline:+9.0f}"
                if baseline and test
                else "-" + spaces(8)
            )

    runs = [
        FormattedRun(idx, baseline_run, test_run)
        for idx, (baseline_run, test_run) in enumerate(
            zip_longest(validation.baseline.times, validation.test.times)
        )
    ]

    table = (
        "| Metric   | Baseline (ns) | Test (ns) | Diff (ns) |\n"
        "|----------|---------------|-----------|-----------|\n" +
        "".join(f"| Run {run.number} | {run.baseline} | {run.test} | {run.diff} |\n" for run in runs) +
        f"| Mean     | {  validation.baseline.mean:13.0f} | {  validation.test.mean:9.0f} | {mean_diff:+4.0f} {regression:>4} |\n"
        f"| Stddev   | {validation.baseline.stddev:13.0f} | {validation.test.stddev:9.0f} | {stddev_diff:+4.0f}      |\n"
    )
    return table

def generate_pull_request_comment(validation: Validation):
    table = format_into_markdown_table(validation)

    comment = (
        "## :warning: Possible LOGR Performance Regression\n\n"
        f"{table}\n"
        f"Mean log time increased {validation.diff:.0f}ns.\n"
    ) if validation.regression else (
        "## :white_check_mark: LOGR Performance Looks OK\n\n"
        f"{table}\n"
        f"Threshold: {validation.threshold:.0f}ns\n"
    )

    with open(COMMENT_FILE, "w") as f:
        f.write(comment)
    print("Wrote", COMMENT_FILE)

def create_baseline(results: Results):
    results.write(BASELINE_FILE)
    print(f"Baseline: {results.mean:.0f}ns ± {results.stddev:.0f}ns")

def test_against_baseline(results: Results):
    results.write(TEST_FILE)

    baseline = parse_results(BASELINE_FILE)

    print(f"Baseline: {baseline.mean:.0f}ns ± {baseline.stddev:.0f}ns")
    print(f"Test:     { results.mean:.0f}ns ± { results.stddev:.0f}ns")

    validation = Validation(baseline, results)
    generate_pull_request_comment(validation)

    print(f"Performance difference per log: {validation.diff:+.0f}ns",
          f"(threshold is {validation.threshold:.0f}ns)")

    return validation.regression

if __name__ == "__main__":
    args = parse_args()

    fail = False

    run(args.exec, WARMUP_REPETITIONS)
    results = run(args.exec, REPETITIONS)

    if args.baseline:
        create_baseline(results)

    elif args.test:
        assert Path(BASELINE_FILE).exists(), (
            f"Baseline file {BASELINE_FILE} not found, cannot run test"
        )

        fail = test_against_baseline(results)

    cleanup_logs()

    sys.exit(int(fail))
