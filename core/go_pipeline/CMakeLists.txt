# Copyright 2022 iLogtail Authors
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

cmake_minimum_required(VERSION 3.22)
project(go_pipeline)

if (LINUX OR MSVC)
    add_library(GoPluginAdapter SHARED LogtailPluginAdapter.h LogtailPluginAdapter.cpp)
    if (LINUX)
        target_link_libraries(GoPluginAdapter dl)
    else ()
        add_definitions(-DPLUGIN_ADAPTER_EXPORTS)
    endif ()
endif ()
