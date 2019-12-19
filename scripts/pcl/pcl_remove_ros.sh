#!/bin/bash
if [ -z "$1" ]; then
  echo "Usage: $0 path/to/project/directory"
  exit 1
fi
PROJECT_ROOT=$1
DIFF_CMD="diff" #input your favorite diff viewer. Examples:
#DIFF_CMD="colordiff" 
#DIFF_CMD="git diff --color-words --no-index"
EDIT_CMD="vim" #input your favorite editor

# Use sed to find and replace classes and namespaces
#Namespaces
PATTERNS="sensor_msgs:pcl std_msgs:pcl"
#Classes
PATTERNS="${PATTERNS} PointField:PCLPointField PointCloud2:PCLPointCloud2 Image:PCLImage Header:PCLHeader"
#Functions
PATTERNS="${PATTERNS} toROSMsg:toPCLPointCloud2 fromROSMsg:fromPCLPointCloud2"
#Moved files
PATTERNS="${PATTERNS} pcl\/ros\/conversions.h:pcl\/conversions.h"

CFILES=`find ${PROJECT_ROOT} -regextype posix-egrep -regex '.*\.h$|.*\.hpp$|.*\.c$|.*\.cpp$'`
if [ "${CFILES}" == "" ]; then
  echo "No C files found"
  exit 1
fi

for PATTERN in $PATTERNS; do
  OLD=`echo ${PATTERN} | sed "s/:.*//g"`
  NEW=`echo ${PATTERN} | sed "s/.*://g"`_CHANGEDWITHSED
  echo "Going from ${OLD} to ${NEW}"
  FILES=`grep -l ${OLD} ${CFILES} | grep -v build`
  for f in ${FILES}; do
    echo "Replacing ${OLD} to ${NEW} in ${f}"
    tmpfile=${f}.tomerge
    if [ ! -e ${tmpfile} ]; then
      cp ${f} ${tmpfile}
    fi
    sed -i "s/${OLD}/${NEW}/g" ${tmpfile}
    sed -i "s/\([a-zA-Z0-9_]\)${NEW}/\\1${OLD}/g" ${tmpfile} #fix broken prefixes
    sed -i "s/${NEW}\([a-zA-Z0-9_]\)/${OLD}\\1/g" ${tmpfile} #fix broken suffixes
    sed -i "s/_CHANGEDWITHSED//g" ${tmpfile}
  done
done
# View all proposed changes
for tmpfile in `find ${PROJECT_ROOT} -name "*.tomerge"`; do
  f=${tmpfile%.tomerge}
  if [ `diff ${f} ${tmpfile} | wc -l` -gt 0 ]; then
    while [ 1 ]; do
      echo ${DIFF_CMD} ${f} ${tmpfile}
      ${DIFF_CMD} ${f} ${tmpfile}
      echo -n "Hit (y) to accept, (n) to reject, (e) to edit, (m) to mark for later: "
      read -n 1 answer
      echo
      if [ "$answer" == "y" ]; then
        mv ${tmpfile} ${f}
        echo MERGED
        break;
      elif [ "$answer" == "n" ]; then
        echo IGNORED
        break;
      elif [ "$answer" == "m" ]; then
        mv ${tmpfile} ${f}.toreview
        echo Proposed change saved to ${f}.toreview
        break;
      elif [ "$answer" == "e" ]; then
        ${EDIT_CMD} ${tmpfile}
      else
        echo "Not a valid option: " $answer
      fi
    done
  fi
  rm -f ${tmpfile}
done
