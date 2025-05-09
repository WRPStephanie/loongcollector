#!/bin/bash
# Copyright 2023 iLogtail Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
set -e

TARGET_ARTIFACT_PATH=${TARGET_ARTIFACT_PATH:-"./core/build/unittest"}

search_files() {
    for file in "$1"/*; do
        if [ -d "$file" ]; then
            # Recursively handle folder
            search_files "$file"
        elif [[ -f "$file" ]]; then
            unittest="${file##*_}"
            if [ "$unittest" == "unittest" ]; then
                echo "[$(date '+%Y-%m-%d %H:%M:%S')] ${file##*/} Start **********"
                cd ${file%/*}
                ./${file##*/}
                cd -
                echo "[$(date '+%Y-%m-%d %H:%M:%S')] ${file##*/} End ############"
                echo
            fi
        fi
    done
}

# Maybe some unittest depend on relative paths, so execute in the unittest directory
UT_BASE_PATH="$(pwd)/${TARGET_ARTIFACT_PATH:2}"
export LD_LIBRARY_PATH=${UT_BASE_PATH}:$LD_LIBRARY_PATH
cd $TARGET_ARTIFACT_PATH
search_files .
