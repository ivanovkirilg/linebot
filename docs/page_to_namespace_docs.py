import argparse

NAMESPACE_DOC_TEMPLATE = """
/// \\namespace {component}
/// \\copybrief {component}_page
/// \\note Description copied from page \\ref {component}_page
/// \\copydetails {component}_page
"""

def parse_args():
    parser = argparse.ArgumentParser(
        description="Generate dummy headers that copy component documentation from a Markdown page to the namespace.")
    parser.add_argument("components", nargs='+', help="List of components for which to generate documentation")
    parser.add_argument("--out-dir", default='.', help="Output directory")
    parser.add_argument("--verbose", action='store_true', help="Print debug output")
    args = parser.parse_args()

    return args

def main(components: list[str], out_dir: str, verbose: bool):
    for component in components:
        out_file = f"{out_dir}/{component}_namespace.hpp"
        if verbose:
            print(f"Generating namespace doc for {component}: {out_file}... ", end='')

        with open(out_file, "w") as f:
            f.write(NAMESPACE_DOC_TEMPLATE.format(component=component))

        if verbose:
            print(f"Done.")

if __name__ == "__main__":
    args = parse_args()
    main(args.components, args.out_dir, args.verbose)
