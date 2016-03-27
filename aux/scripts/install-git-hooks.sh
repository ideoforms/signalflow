#!/bin/bash

#------------------------------------------------------------------------
# install-git-hooks.sh:
#
# Installs a git-hook to automatically increment SIGNAL_BUILD
# each time a new commit is created.
#------------------------------------------------------------------------

GIT_DIR=$(git rev-parse --git-dir)
GIT_HOOK_DIR="$GIT_DIR/hooks"
GIT_HOOK_SCRIPT_TARGET="$GIT_DIR/hooks/pre-commit"

#------------------------------------------------------------------------
# Don't overwrite existing hooks.
#------------------------------------------------------------------------
if [ -e "$GIT_HOOK_SCRIPT_TARGET" ]
then
	echo "Hook script $GIT_HOOK_SCRIPT_TARGET already exists, not overwriting."
	exit 1
fi

#------------------------------------------------------------------------
# Create our pre-commit script.
#------------------------------------------------------------------------
read -r -d '' SCRIPT <<EOF
#!/bin/bash

GIT_REPO_DIR=\$(git rev-parse --show-toplevel)
VERSION_H="\$GIT_REPO_DIR/signal/version.h"

if ! grep -q SIGNAL_BUILD "\$VERSION_H"
then
	echo "Couldn't find SIGNAL_BUILD marker."
	exit 1
fi
echo "Processing \$VERSION_H"

# Increment build number
perl -i -pe 's/(SIGNAL_BUILD) (\d+)/"\$1 " . (\$2 + 1)/e' "\$VERSION_H"
git add \$VERSION_H
EOF

#------------------------------------------------------------------------
# Write the script.
#------------------------------------------------------------------------
echo "$SCRIPT" > $GIT_HOOK_SCRIPT_TARGET
chmod +x "$GIT_HOOK_SCRIPT_TARGET"

echo "Installed git hook: "
echo "$GIT_HOOK_SCRIPT_TARGET"
