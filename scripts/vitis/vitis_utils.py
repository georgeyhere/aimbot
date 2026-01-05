

import os

def symlink_app_sources(app_source_dir, dest_dir, app_name=None):
    """
    Recursively create relative symlinks for all .c/.cpp/.h/.hpp files from app_source_dir into dest_dir, preserving directory structure.
    """
    print(f"Creating links for all source files in {app_source_dir}")
    relevant_file_extensions = ['.cpp', '.c', '.h', '.hpp']
    for root, dirs, files in os.walk(app_source_dir):
        rel_root = os.path.relpath(root, app_source_dir)
        dest_subdir = os.path.join(dest_dir, rel_root) if rel_root != '.' else dest_dir
        os.makedirs(dest_subdir, exist_ok=True)
        for filename in files:
            if not any(filename.endswith(ext) for ext in relevant_file_extensions):
                continue
            target_file = os.path.realpath(os.path.join(root, filename))
            link_file = os.path.realpath(os.path.join(dest_subdir, filename))
            rel_target = os.path.relpath(target_file, os.path.dirname(link_file))
            print(f"Creating soft link '{link_file}' -> '{rel_target}'")
            try:
                if os.path.lexists(link_file):
                    os.remove(link_file)
                os.symlink(rel_target, link_file)
            except Exception as e:
                print(f"Failed to create symlink for {filename}: {e}")