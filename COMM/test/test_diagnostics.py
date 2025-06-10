import argparse
import subprocess
import html

from dataclasses import dataclass
from pathlib import Path

HTML_TEMPLATE = """
<!DOCTYPE html>
<html>
<head>
    <title>Interface Generator Diagnostics</title>
    <style>
        /* Base styles */
        body {{
            font: 0.9em/1.4 Arial, sans-serif;
            max-width: 1200px;
            margin: 1em auto;
        }}
        pre {{
            margin: 0;
            padding: 0.5em;
            overflow-x: auto;
        }}

        /* Group styling */
        .group {{
            margin-bottom: 1em;
            border: 1px solid #888;
            border-radius: 4px;
        }}
        .group-header {{
            background: #4a4a4a;
            color: white;
            padding: 0.5em;
            cursor: pointer;
            user-select: none;
        }}
        .group-header:hover {{ background: #5a5a5a; }}
        .group-content {{
            padding: 0.5em;
            display: none;
        }}
        .group-content.expanded {{ display: block; }}

        /* File container */
        .file {{
            margin-bottom: 1em;
            border: 1px solid #ccc;
            padding: 0.5em;
        }}
        .file h2 {{
            margin: 0 0 0.5em;
            font-size: 1.2em;
        }}

        /* Content areas */
        .source {{ background: #f5f5f5; }}
        .output {{ background: #fff8dc; }}
        .error {{ background: #ffe4e1; }}
    </style>
    <script>
        const toggleGroup = id => document.getElementById(id).classList.toggle('expanded');
    </script>
</head>
<body>
    <h1>Interface Generator Diagnostics</h1>
    {content}
</body>
</html>
"""

GROUP_TEMPLATE = """
    <div class="group">
        <div class="group-header" onclick="toggleGroup('group_{group_id}')">
            📁 {group_name} ({file_count} files)
        </div>
        <div class="group-content" id="group_{group_id}">
            {files}
        </div>
    </div>
"""

FILE_TEMPLATE = """
    <div class="file">
        <h2>{filename}</h2>
        <pre class="source">{source}</pre>
        <pre class="output">{output}</pre>
        <pre class="error">{error}</pre>
    </div>
"""

@dataclass
class ExecutionData:
    path: Path
    source: str
    stdout: str
    stderr: str
    retcode: int

def parse_arguments():
    arg_parser = argparse.ArgumentParser(description=
        'Execute interface_generator on a number of interface files and '
        'generate an HTML report for human inspection of the quality '
        'of the diagnostic messages.')
    arg_parser.add_argument('directories', nargs='+',
                            help='Directories containing .interface files')
    arg_parser.add_argument('--web-output', '-w', default='interface_diagnostics.html',
                            help='Output HTML file path')
    arg_parser.add_argument('--md-output', '-m', default=None,
                            help='Output markdown file path')
    return arg_parser.parse_args()

def process_directory(directory: Path) -> list[ExecutionData]:
    executions = []

    for file in sorted(directory.glob('*.interface')):
        with open(file, 'r') as f:
            source = f.read()

        cmd = ['python3', '-m', 'interface_generator', str(file), '-n', 'TEST', '-o', '/tmp']
        generator = subprocess.Popen(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        out, err = generator.communicate()
        print('.', end='', flush=True)

        execution = ExecutionData(file, source, out, err, generator.returncode)
        executions.append(execution)

    return executions

def generate_directory_html(executions: list[ExecutionData],
                            dir_id: int,
                            dir_name: str) -> str:
    files = [
        FILE_TEMPLATE.format(
            filename=html.escape(exec.path.name),
            source=html.escape(exec.source),
            output=(html.escape(exec.stdout) or "<i>No stdout</i>"),
            error=(html.escape(exec.stderr) or "<i>No stderr</i>")
        )
        for exec in executions
    ]
    return GROUP_TEMPLATE.format(
        group_id=dir_id,
        group_name=dir_name,
        file_count=len(executions),
        files='\n'.join(files)
    )

def generate_directory_text(executions: list[ExecutionData],
                            dir_name: str) -> str:
    indent = '\n    '

    out = f'\n# {dir_name}'
    for exec in executions:
        out += f'\n## {exec.path.name}'
        out += f'\nExited with code {exec.retcode}'
        out += f'\n### Source'
        out += indent + indent.join(exec.source.splitlines())
        out += f'\n### Stdout'
        out += indent + indent.join(exec.stdout.splitlines())
        out += f'\n### Stderr'
        out += indent + indent.join(exec.stderr.splitlines())
        out += '\n'

    return out

def generate_report(directories: list[str], html_out: str, md_out: str):
    html_content = []
    md_content = []

    for dir_id, dir in enumerate(directories):
        dir_path = Path(dir)
        if not dir_path.is_dir():
            print(f"Warning: {dir} is not a directory, skipping...")
            continue

        print(f'{dir_path.name}: ', end='')
        executions = process_directory(dir_path)
        if executions:
            html_content.append(
                generate_directory_html(executions, dir_id, dir_path.name)
            )
            md_content.append(
                generate_directory_text(executions, dir_path.name)
            )
        else:
            print(f"Warning: {dir} is empty.")
        print('')

    with open(html_out, 'w') as f:
        f.write(HTML_TEMPLATE.format(content='\n'.join(html_content)))
    print(f"Report generated at: {Path(html_out).resolve()}")

    if md_out:
        with open(md_out, 'w') as f:
            f.write('\n'.join(md_content))
        print(f"Report generated at: {Path(md_out).resolve()}")


if __name__ == '__main__':
    args = parse_arguments()
    generate_report(args.directories, args.web_output, args.md_output)
