import subprocess
import re
import sys

_TEST_LIST_REGEX = re.compile(r'Test +#\d+: (.+)')

def list_label(label: str) -> list[str]:
    try:
        result = subprocess.run(
            ["ctest", "--test-dir", "build/", "-N", "-L", label],
            capture_output=True,
            text=True,
            check=True
        )

        return re.findall(_TEST_LIST_REGEX, result.stdout)

    except subprocess.CalledProcessError as e:
        print(f"Error running ctest: {e}", file=sys.stderr)
        raise

def run(test_regex: str) -> int:
    result = subprocess.run(["ctest", "--test-dir", "build/", "-R", test_regex])
    return result.returncode
