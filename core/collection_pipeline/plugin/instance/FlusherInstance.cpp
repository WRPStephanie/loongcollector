// Copyright 2023 iLogtail Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "collection_pipeline/plugin/instance/FlusherInstance.h"

#include "monitor/metric_constants/MetricConstants.h"

using namespace std;

namespace logtail {

bool FlusherInstance::Init(const Json::Value& config,
                           CollectionPipelineContext& context,
                           size_t flusherIdx,
                           Json::Value& optionalGoPipeline) {
    mPlugin->SetContext(context);
    mPlugin->SetPluginID(PluginID());
    mPlugin->SetFlusherIndex(flusherIdx);
    mPlugin->CreateMetricsRecordRef(Name(), PluginID());
    if (!mPlugin->Init(config, optionalGoPipeline)) {
        return false;
    }

    mInGroupsTotal = mPlugin->GetMetricsRecordRef().CreateCounter(METRIC_PLUGIN_IN_EVENT_GROUPS_TOTAL);
    mInEventsTotal = mPlugin->GetMetricsRecordRef().CreateCounter(METRIC_PLUGIN_IN_EVENTS_TOTAL);
    mInSizeBytes = mPlugin->GetMetricsRecordRef().CreateCounter(METRIC_PLUGIN_IN_SIZE_BYTES);
    mTotalPackageTimeMs = mPlugin->GetMetricsRecordRef().CreateTimeCounter(METRIC_PLUGIN_FLUSHER_TOTAL_PACKAGE_TIME_MS);
    mPlugin->CommitMetricsRecordRef();
    return true;
}

bool FlusherInstance::Send(PipelineEventGroup&& g) {
    ADD_COUNTER(mInGroupsTotal, 1);
    ADD_COUNTER(mInEventsTotal, g.GetEvents().size());
    ADD_COUNTER(mInSizeBytes, g.DataSize());

    auto before = chrono::system_clock::now();
    auto res = mPlugin->Send(std::move(g));
    ADD_COUNTER(mTotalPackageTimeMs, chrono::system_clock::now() - before);
    return res;
}

} // namespace logtail
