// Copyright 2024 iLogtail Authors
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


#include "PipelineEventGroup.h"
#include "PipelineEventPtr.h"
#include "collection_pipeline/batch/BatchStatus.h"
#include "collection_pipeline/batch/FlushStrategy.h"
#include "unittest/Unittest.h"

using namespace std;

namespace logtail {

class EventFlushStrategyUnittest : public ::testing::Test {
public:
    void TestNeedFlush();

protected:
    void SetUp() override {
        mStrategy.SetMinCnt(2);
        mStrategy.SetMaxSizeBytes(200);
        mStrategy.SetMinSizeBytes(100);
        mStrategy.SetTimeoutSecs(3);
    }

private:
    EventFlushStrategy<EventBatchStatus> mStrategy;
};

void EventFlushStrategyUnittest::TestNeedFlush() {
    EventBatchStatus status;

    status.mCnt = 2;
    status.mSizeBytes = 50;
    status.mCreateTime = time(nullptr) - 1;
    APSARA_TEST_TRUE(mStrategy.NeedFlushByCnt(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushBySize(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, PipelineEventPtr()));
    APSARA_TEST_FALSE(mStrategy.SizeReachingUpperLimit(status));

    status.mCnt = 1;
    status.mSizeBytes = 100;
    status.mCreateTime = time(nullptr) - 1;
    APSARA_TEST_FALSE(mStrategy.NeedFlushByCnt(status));
    APSARA_TEST_TRUE(mStrategy.NeedFlushBySize(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, PipelineEventPtr()));
    APSARA_TEST_FALSE(mStrategy.SizeReachingUpperLimit(status));

    status.mCnt = 1;
    status.mSizeBytes = 50;
    status.mCreateTime = time(nullptr) - 4;
    APSARA_TEST_FALSE(mStrategy.NeedFlushByCnt(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushBySize(status));
    APSARA_TEST_TRUE(mStrategy.NeedFlushByTime(status, PipelineEventPtr()));
    APSARA_TEST_FALSE(mStrategy.SizeReachingUpperLimit(status));

    status.mSizeBytes = 300;
    status.mCreateTime = time(nullptr) - 1;
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, PipelineEventPtr()));
    APSARA_TEST_TRUE(mStrategy.SizeReachingUpperLimit(status));
}

UNIT_TEST_CASE(EventFlushStrategyUnittest, TestNeedFlush)

class GroupFlushStrategyUnittest : public ::testing::Test {
public:
    void TestNeedFlush();
};

void GroupFlushStrategyUnittest::TestNeedFlush() {
    GroupFlushStrategy strategy(100, 3);
    GroupBatchStatus status;

    status.mSizeBytes = 100;
    status.mCreateTime = time(nullptr) - 1;
    APSARA_TEST_TRUE(strategy.NeedFlushBySize(status));
    APSARA_TEST_FALSE(strategy.NeedFlushByTime(status));

    status.mSizeBytes = 50;
    status.mCreateTime = time(nullptr) - 4;
    APSARA_TEST_FALSE(strategy.NeedFlushBySize(status));
    APSARA_TEST_TRUE(strategy.NeedFlushByTime(status));
}

UNIT_TEST_CASE(GroupFlushStrategyUnittest, TestNeedFlush)

class SLSEventFlushStrategyUnittest : public ::testing::Test {
public:
    void TestNeedFlush();

protected:
    void SetUp() override {
        mStrategy.SetMinCnt(2);
        mStrategy.SetMinSizeBytes(100);
        mStrategy.SetTimeoutSecs(3);
    }

private:
    EventFlushStrategy<SLSEventBatchStatus> mStrategy;
};

void SLSEventFlushStrategyUnittest::TestNeedFlush() {
    PipelineEventGroup eventGroup(make_shared<SourceBuffer>());
    PipelineEventPtr event(eventGroup.CreateLogEvent(), false, nullptr);
    event->SetTimestamp(1717398001);
    PipelineEventPtr metricEvent(eventGroup.CreateMetricEvent(), false, nullptr);
    metricEvent->SetTimestamp(time(nullptr));

    SLSEventBatchStatus status;
    status.mCnt = 2;
    status.mSizeBytes = 50;
    status.mCreateTime = time(nullptr) - 1;
    status.mCreateTimeMinute = 1717398001 / 60;
    APSARA_TEST_TRUE(mStrategy.NeedFlushByCnt(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushBySize(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, event));
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, metricEvent));

    status.mCnt = 1;
    status.mSizeBytes = 100;
    status.mCreateTime = time(nullptr) - 1;
    status.mCreateTimeMinute = 1717398001 / 60;
    APSARA_TEST_FALSE(mStrategy.NeedFlushByCnt(status));
    APSARA_TEST_TRUE(mStrategy.NeedFlushBySize(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, event));
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, metricEvent));

    status.mCnt = 1;
    status.mSizeBytes = 50;
    status.mCreateTime = time(nullptr) - 4;
    status.mCreateTimeMinute = 1717398001 / 60;
    APSARA_TEST_FALSE(mStrategy.NeedFlushByCnt(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushBySize(status));
    APSARA_TEST_TRUE(mStrategy.NeedFlushByTime(status, event));
    APSARA_TEST_TRUE(mStrategy.NeedFlushByTime(status, metricEvent));

    status.mCnt = 1;
    status.mSizeBytes = 50;
    status.mCreateTime = time(nullptr) - 1;
    status.mCreateTimeMinute = 1717398071 / 60;
    APSARA_TEST_FALSE(mStrategy.NeedFlushByCnt(status));
    APSARA_TEST_FALSE(mStrategy.NeedFlushBySize(status));
    APSARA_TEST_TRUE(mStrategy.NeedFlushByTime(status, event));
    APSARA_TEST_FALSE(mStrategy.NeedFlushByTime(status, metricEvent));
    metricEvent->SetTimestamp(time(nullptr) - 302);
    APSARA_TEST_TRUE(mStrategy.NeedFlushByTime(status, metricEvent));
    metricEvent->SetTimestamp(time(nullptr) + 301);
    APSARA_TEST_TRUE(mStrategy.NeedFlushByTime(status, metricEvent));
}

UNIT_TEST_CASE(SLSEventFlushStrategyUnittest, TestNeedFlush)

} // namespace logtail

UNIT_TEST_MAIN
