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
project(processor_unittest)

add_executable(processor_split_log_string_native_unittest ProcessorSplitLogStringNativeUnittest.cpp)
target_link_libraries(processor_split_log_string_native_unittest ${UT_BASE_TARGET})

add_executable(processor_split_multiline_log_string_native_unittest ProcessorSplitMultilineLogStringNativeUnittest.cpp)
target_link_libraries(processor_split_multiline_log_string_native_unittest ${UT_BASE_TARGET})

add_executable(processor_parse_regex_native_unittest ProcessorParseRegexNativeUnittest.cpp)
target_link_libraries(processor_parse_regex_native_unittest ${UT_BASE_TARGET})

add_executable(processor_parse_json_native_unittest ProcessorParseJsonNativeUnittest.cpp)
target_link_libraries(processor_parse_json_native_unittest ${UT_BASE_TARGET})

add_executable(processor_parse_timestamp_native_unittest ProcessorParseTimestampNativeUnittest.cpp)
target_link_libraries(processor_parse_timestamp_native_unittest ${UT_BASE_TARGET})

add_executable(processor_tag_native_unittest ProcessorTagNativeUnittest.cpp)
target_link_libraries(processor_tag_native_unittest ${UT_BASE_TARGET})

add_executable(processor_parse_apsara_native_unittest ProcessorParseApsaraNativeUnittest.cpp)
target_link_libraries(processor_parse_apsara_native_unittest ${UT_BASE_TARGET})

add_executable(processor_parse_delimiter_native_unittest ProcessorParseDelimiterNativeUnittest.cpp)
target_link_libraries(processor_parse_delimiter_native_unittest ${UT_BASE_TARGET})

add_executable(processor_filter_native_unittest ProcessorFilterNativeUnittest.cpp)
target_link_libraries(processor_filter_native_unittest ${UT_BASE_TARGET})

add_executable(processor_desensitize_native_unittest ProcessorDesensitizeNativeUnittest.cpp)
target_link_libraries(processor_desensitize_native_unittest ${UT_BASE_TARGET})

add_executable(processor_merge_multiline_log_native_unittest ProcessorMergeMultilineLogNativeUnittest.cpp)
target_link_libraries(processor_merge_multiline_log_native_unittest ${UT_BASE_TARGET})

add_executable(boost_regex_benchmark BoostRegexBenchmark.cpp)
target_link_libraries(boost_regex_benchmark ${UT_BASE_TARGET})

if (LINUX)
    add_executable(processor_prom_relabel_metric_native_unittest ProcessorPromRelabelMetricNativeUnittest.cpp)
    target_link_libraries(processor_prom_relabel_metric_native_unittest unittest_base)

    add_executable(processor_prom_parse_metric_native_unittest ProcessorPromParseMetricNativeUnittest.cpp)
    target_link_libraries(processor_prom_parse_metric_native_unittest unittest_base)

    add_executable(processor_parse_container_log_native_unittest ProcessorParseContainerLogNativeUnittest.cpp)
    target_link_libraries(processor_parse_container_log_native_unittest ${UT_BASE_TARGET})

    add_executable(parse_container_log_benchmark ParseContainerLogBenchmark.cpp)
    target_link_libraries(parse_container_log_benchmark ${UT_BASE_TARGET})
endif()

include(GoogleTest)
gtest_discover_tests(processor_split_log_string_native_unittest)
gtest_discover_tests(processor_split_multiline_log_string_native_unittest)
gtest_discover_tests(processor_parse_regex_native_unittest)
gtest_discover_tests(processor_parse_json_native_unittest)
gtest_discover_tests(processor_parse_timestamp_native_unittest)
gtest_discover_tests(processor_tag_native_unittest)
gtest_discover_tests(processor_parse_apsara_native_unittest)
gtest_discover_tests(processor_parse_delimiter_native_unittest)
gtest_discover_tests(processor_filter_native_unittest)
gtest_discover_tests(processor_desensitize_native_unittest)
gtest_discover_tests(processor_merge_multiline_log_native_unittest)
if (LINUX)
    gtest_discover_tests(processor_prom_relabel_metric_native_unittest)
    gtest_discover_tests(processor_prom_parse_metric_native_unittest)
    gtest_discover_tests(processor_parse_container_log_native_unittest)
endif()
