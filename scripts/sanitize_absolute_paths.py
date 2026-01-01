import os
import sys

def get_gitignore_patterns(gitignore_path):
	patterns = set()
	if os.path.isfile(gitignore_path):
		with open(gitignore_path, 'r') as f:
			for line in f:
				line = line.strip()
				if line and not line.startswith('#'):
					patterns.add(line)
	return patterns

def is_ignored(file_path, patterns, base_dir):
	rel_path = os.path.relpath(file_path, base_dir)
	for pattern in patterns:
		# Simple pattern match (not full .gitignore spec)
		if pattern.endswith('/'):
			if rel_path.startswith(pattern):
				return True
		elif pattern in rel_path:
			return True
	return False

def sanitize_paths(directory, workroot, ignore_patterns):
	for root, dirs, files in os.walk(directory):
		for file in files:
			file_path = os.path.join(root, file)
			if is_ignored(file_path, ignore_patterns, directory):
				continue
			try:
				with open(file_path, 'r', encoding='utf-8') as f:
					lines = f.readlines()
			except Exception:
				continue
			updated = False
			new_lines = []
			for idx, line in enumerate(lines):
				if workroot in line:
					print(f"[sanitize] {file_path}: line {idx+1} updated")
					line = line.replace(workroot, '$WORKROOT')
					updated = True
				new_lines.append(line)
			if updated:
				with open(file_path, 'w', encoding='utf-8') as f:
					f.writelines(new_lines)

def setup_paths(directory, workroot, ignore_patterns):
	for root, dirs, files in os.walk(directory):
		for file in files:
			file_path = os.path.join(root, file)
			if is_ignored(file_path, ignore_patterns, directory):
				continue
			try:
				with open(file_path, 'r', encoding='utf-8') as f:
					lines = f.readlines()
			except Exception:
				continue
			updated = False
			new_lines = []
			for idx, line in enumerate(lines):
				if '$WORKROOT' in line:
					print(f"[setup] {file_path}: line {idx+1} updated")
					line = line.replace('$WORKROOT', workroot)
					updated = True
				new_lines.append(line)
			if updated:
				with open(file_path, 'w', encoding='utf-8') as f:
					f.writelines(new_lines)

if __name__ == "__main__":

		def print_help():
				print("""
sanitize_absolute_paths.py

This Python script is used to sanitize or restore absolute paths in Vitis project files.
Vitis 2025.2 supports git integration.
However, some project files may still contain absolute paths to the workspace.

This script is used to 'sanitize' absolute paths by replacing them with '$WORKROOT' prior to committing.
When the project is checked out, the script can be used in 'setup' mode to restore the absolute paths 
based on the current WORKROOT environment variable.

If $WORKROOT is not set, the script will exit with an error.
In this case, run `source set_workroot.sh` from the repo root first.

Usage:
	python sanitize_absolute_paths.py <prj_name> <mode>

Arguments:
	<prj_name>   Name of the project under prj/<prj_name>/vitis
	<mode>       'sanitize' to replace absolute paths with $WORKROOT
				 'setup' to replace $WORKROOT with the actual path

Options:
	-h, --help   Show this help message and exit
""")

		if len(sys.argv) == 2 and sys.argv[1] in ('-h', '--help'):
				print_help()
				sys.exit(0)
		if len(sys.argv) != 3:
				print_help()
				sys.exit(1)

	prj_name = sys.argv[1]
	mode = sys.argv[2]
	workroot = os.environ.get('WORKROOT')
	if not workroot:
		print("Error: WORKROOT environment variable is not set.")
		sys.exit(1)

	target_dir = os.path.join('prj', prj_name, 'vitis')
	if not os.path.isdir(target_dir):
		print(f"Error: Directory {target_dir} does not exist.")
		sys.exit(1)

	gitignore_path = os.path.join(target_dir, '.gitignore')
	ignore_patterns = get_gitignore_patterns(gitignore_path)

	if mode == 'sanitize':
		sanitize_paths(target_dir, workroot, ignore_patterns)
	elif mode == 'setup':
		setup_paths(target_dir, workroot, ignore_patterns)
	else:
		print("Error: mode must be 'sanitize' or 'setup'.")
		sys.exit(1)
