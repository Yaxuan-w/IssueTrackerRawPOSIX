import json
import re
import argparse
from subprocess import Popen, PIPE, STDOUT

parser = argparse.ArgumentParser(
    description="Tests for SFI penalty in Lind project"
)
parser.add_argument(
    "-e",
    "--execution-command",
    dest="command",
    type=str,
    required=True,
    help="Platform specific execution command",
)
parser.add_argument(
    "-c",
    "--count",
    dest="count",
    type=int,
    default=10,
    help="Number of runs",
)
args = parser.parse_args()

run_times = {}
run_times[args.count] = []

for _ in range(args.count):
    command = args.command.split()
    output = Popen(
        command,
        stdout=PIPE,
        stderr=STDOUT,
    )
    stdout, _ = output.communicate()
    stdout = stdout.decode('utf-8')
    match = re.search(r'average time (\d+) ns', stdout)
    if match:
        run_time = int(match.group(1))
        run_times[args.count].append(run_time)
        print(f"Average time {run_time} ns ")
    else:
        print("Average time not found in the output.")

with open('average_time.json', 'w') as fp:
    json.dump(run_times, fp)