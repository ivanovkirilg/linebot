import requests

CI_PROJECT_ID = 'CI_PROJECT_ID'
CI_MERGE_REQUEST_IID = 'CI_MERGE_REQUEST_IID'

# This one is a custom secret (Project Access Token)
CI_PRIVATE_TOKEN = 'CI_PRIVATE_TOKEN'

_NOTES_URL = "https://gitlab.com/api/v4/projects/{}/merge_requests/{}/notes"

class HiddenStr:
    def __init__(self, value: str):
        self.__value = value

    def reveal(self):
        return self.__value

    def __repr__(self):
        return "'<REDACTED>'"

    def __str__(self):
        return "'<REDACTED>'"

class NotesApi:
    @staticmethod
    def from_env():
        import os

        assert CI_PROJECT_ID in os.environ and \
               CI_MERGE_REQUEST_IID in os.environ, \
               f"Not running in Merge Request context."

        assert CI_PRIVATE_TOKEN in os.environ, \
               f"CI_PRIVATE_TOKEN environment variable is not set."

        ci_project_id = os.environ[CI_PROJECT_ID]
        ci_merge_request_iid = os.environ[CI_MERGE_REQUEST_IID]
        ci_private_token = HiddenStr(os.environ[CI_PRIVATE_TOKEN])

        return NotesApi(ci_project_id, ci_merge_request_iid, ci_private_token)

    def __init__(self, ci_project_id: str, ci_merge_request_iid: str, ci_private_token: HiddenStr):
        self.url = _NOTES_URL.format(ci_project_id, ci_merge_request_iid)
        self.__ci_private_token = ci_private_token

    def __build_request(self, url = None):
        return {
            'headers': {'PRIVATE-TOKEN': self.__ci_private_token.reveal()},
            'url': url if url else self.url,
        }

    def fetch_note(self, id) -> dict:
        url = f"{self.url}/{id}"

        response = requests.get(
            **self.__build_request(url)
        )
        response.raise_for_status()
        return response.json()

    def fetch_page(self, page=1) -> list[dict]:
        url = f"{self.url}?page={page}"

        response = requests.get(
            **self.__build_request(url)
        )
        response.raise_for_status()
        return response.json()

    def fetch_all(self) -> list[dict]:
        notes = []
        page = iter(range(1, 1000))

        page_notes = self.fetch_page(next(page))
        while page_notes:
            notes.extend(page_notes)
            page_notes = self.fetch_page(next(page))

        return notes

    def find_matching(self, substring: str):
        for note in self.fetch_all():
            if substring in note["body"]:
                return note

    def find_all_matching(self, substring: str) -> list[dict]:
        return [note for note in self.fetch_all() if substring in note["body"]]

    def create(self, body: str) -> dict:
        response = requests.post(
            data={"body": body},
            **self.__build_request()
        )
        response.raise_for_status()
        return response.json()

    def update(self, body: str, id) -> dict:
        url = f"{self.url}/{id}"

        response = requests.put(
            data={"body": body},
            **self.__build_request(url)
        )
        response.raise_for_status()
        return response.json()

    def create_sticky(self, body: str, tag: str):
        old_note = self.find_matching(tag)

        if old_note:
            print(f"Updating existing comment {old_note['id']}...")
            self.update(body, old_note["id"])
        else:
            print("Posting comment...")
            self.create(body)
