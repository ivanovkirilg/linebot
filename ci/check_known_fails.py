#!/usr/bin/env python3

import argparse

from dataclasses import dataclass, field

import ctest
from gitlab import NotesApi

COMMENT_TAG = "[pipeline-comment known-fail-passed-{}]: #"

@dataclass
class Results:
    build_type: str
    passed: list[str] = field(default_factory=list)

    def format_comment(self):
        if not self.passed:
            return ''

        message = f"The following tests have been labeled as a `known-fail` "
        message += f"but **passed** on the `{self.build_type}` build:\n\n"
        message += '\n'.join(f"- `{test}`" for test in self.passed)
        message += f"\n\n{COMMENT_TAG.format(self.build_type)}\n"

        return message

    def write_comment(self):
        with open(f"known-fail-passed-{self.build_type}.md", "w") as f:
            f.write(self.format_comment())

def parse_args():
    parser = argparse.ArgumentParser(description="Check known-fail tests that unexpectedly pass")
    parser.add_argument("build_type", help="CMake build type (e.g. Debug, RelWithDebInfo)")
    args = parser.parse_args()

    return args

def run_known_fails(build_type: str) -> Results:
    known_fails = ctest.list_label("known-fail")
    print(f"Checking known-fails {build_type} build: {', '.join(known_fails)}")

    results = Results(build_type)

    for known_fail in known_fails:
        if ctest.run(known_fail) == 0:
            print(f"Test '{known_fail}' passed unexpectedly.")
            results.passed.append(known_fail)
        else:
            print(f"Test '{known_fail}' failed as expected.")

    return results

def main(build_type):
    results = run_known_fails(build_type)

    if results.passed:
        results.write_comment()
        comment = results.format_comment()

        notes_api = NotesApi.from_env()

        notes_api.create_sticky(comment, COMMENT_TAG.format(build_type))

if __name__ == "__main__":
    args = parse_args()
    main(args.build_type)
