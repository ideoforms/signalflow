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
GITHUB_CHANGELOG_GENERATOR=`which github_changelog_generator`

#------------------------------------------------------------------------
# Don't overwrite existing hooks.
#------------------------------------------------------------------------
if [ -e "$GIT_HOOK_SCRIPT_TARGET" ]
then
	echo "Hook script $GIT_HOOK_SCRIPT_TARGET already exists, not overwriting."
	exit 1
fi

#------------------------------------------------------------------------
# Don't overwrite existing hooks.
#------------------------------------------------------------------------
if [ "$GITHUB_CHANGELOG_GENERATOR" = "" ]
then
	echo "Requires github_changelog_generator. Please install and retry."
	exit 1
fi

#------------------------------------------------------------------------
# Create our pre-commit script.
#------------------------------------------------------------------------
read -r -d '' SCRIPT <<EOF
#!/bin/bash

GIT_BRANCH=\$(git symbolic-ref --short -q HEAD)
GIT_REPO_DIR=\$(git rev-parse --show-toplevel)

#------------------------------------------------------------------------
# If we're editing web content, don't try to increment build counter
#------------------------------------------------------------------------
if [ "\$GIT_BRANCH" = "gh-pages" ]
then
    exit 0
fi

VERSION_H="\$GIT_REPO_DIR/source/include/signalflow/core/version.h"
CURRENT_BUILD=\`cat \$VERSION_H | grep SIGNAL_BUILD | cut -d' ' -f3\`

if [ "\$CURRENT_BUILD" = "" ]
then
	echo "Couldn't find SIGNAL_BUILD marker."
	exit 1
fi
CURRENT_BUILD=\$((CURRENT_BUILD + 1))
echo "Incrementing SIGNAL_BUILD to \$CURRENT_BUILD"

# Increment build number
perl -i -pe 's/(SIGNAL_BUILD) (\d+)/"\$1 " . (\$2 + 1)/e' "\$VERSION_H"
git add \$VERSION_H

# Auto-generate CHANGELOG
$GITHUB_CHANGELOG_GENERATOR --no-verbose
git add CHANGELOG.md
EOF

#------------------------------------------------------------------------
# Write the script.
#------------------------------------------------------------------------
echo "$SCRIPT" > $GIT_HOOK_SCRIPT_TARGET
chmod +x "$GIT_HOOK_SCRIPT_TARGET"

echo "Installed git hook: "
echo "$GIT_HOOK_SCRIPT_TARGET"
