#!/bin/bash

#
# Script to rename varnish-template
#
# Leaves the changes uncommitted in the working tree, so you get to fix any
# unintended changes.
#
# Author: Nils Goroll <slink@uplex.de>, October 2017
# based upon prior work by:
# Author: Lasse Karstensen <lkarsten@varnish-software.com>, September 2014.

set -eux

NAME=${1:-}
GH=${2:-}

if [[ -z "${NAME}" ]] || [[ -z "${GH}" ]] ; then
   cat <<EOF
Usage: $0 <name> <github-user>

Clone and rename varnish-template to your own vmod/tool name.
EOF
    exit -1
fi

SYM_NAME=${NAME,,*}
CAP_NAME=${NAME^?}
UPP_NAME=${NAME^^?}

if ! git diff-index --quiet HEAD --; then
	echo "ERROR: Need a clean working tree. Run \"git stash\" first."
	exit -2
fi

(
    git ls-files | grep / | sed 's:/[^/]*$::'| sort -ru &&
	git ls-files
) | \
    while read f ; do
	if ! [[ $(basename "${f}") == *template* ]] ; then
	    continue
	fi
	n=${f//template/${NAME}}
	git mv "${f}" "${n}"
    done

git grep -z -l template | xargs -0 sed -i -s -e "s/template/${SYM_NAME}/g"
git grep -z -l Template | xargs -0 sed -i -s -e "s/Template/${CAP_NAME}/g"
git grep -z -l TEMPLATE | xargs -0 sed -i -s -e "s/TEMPLATE/${UPP_NAME}/g"
git grep -z -l github.com/dridi | \
    xargs -0 sed -i -s -e "s/github\.com\/dridi/github.com\/${GH}/g"

git rm -f rename.sh

# restore foreign files
git checkout -- m4/

git commit -a -m "Automatic rename of varnishtemplate to ${SYM_NAME}."
