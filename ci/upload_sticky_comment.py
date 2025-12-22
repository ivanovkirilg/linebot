import argparse
import os.path
import re

import gitlab

TAG_REGEX = re.compile(r'\[pipeline-comment [a-zA-Z0-9\-_]+\]: #')

def parse_args():
    parser = argparse.ArgumentParser(description="Upload sticky comment to GitLab MR.")
    parser.add_argument("--file", type=str, required=True,
                        help="Path to the comment file to upload.")
    parser.add_argument("--tag", type=str,
                        help="Tag to match against; if not given, search for one in the comment file.")
    parser.add_argument("--optional", action="store_true",
                        help="Not an error if the comment file does not exist.")
    return parser.parse_args()

def find_tag_in_comment(comment: str) -> str:
    match = TAG_REGEX.search(comment)
    if match is None:
        raise ValueError(f"Could not find tag.")
    return match.group(0)

def main(comment_file: str, tag: str | None, is_optional: bool):
    if os.path.exists(comment_file):
        with open(comment_file, "r") as f:
            comment = f.read()

        if tag is None:
            tag = find_tag_in_comment(comment)

        notes_api = gitlab.NotesApi.from_env()
        notes_api.create_sticky(comment, tag)

    else:
        if is_optional:
            print(f"Comment file '{comment_file}' does not exist, skipping upload.")
        else:
            raise FileNotFoundError(f"Comment file '{comment_file}' does not exist.")

if __name__ == "__main__":
    args = parse_args()
    main(args.file, args.tag, args.optional)
