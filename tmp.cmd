git filter-branch --prune-empty --index-filter 'git rm --cached --ignore-unmatch path/to/file' --all