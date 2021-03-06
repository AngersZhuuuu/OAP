/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <arrow/builder.h>
#include <arrow/compute/context.h>
#include <arrow/status.h>
#include <arrow/type.h>
#include <arrow/type_fwd.h>
#include <arrow/type_traits.h>
#include <arrow/util/checked_cast.h>
#include <math.h>
#include <limits>

#include <iostream>
#include <memory>
#include <sstream>

namespace sparkcolumnarplugin {
namespace codegen {
namespace arrowcompute {
namespace extra {

using ArrayList = std::vector<std::shared_ptr<arrow::Array>>;

class ActionBase {
 public:
  virtual ~ActionBase() {}

  virtual int RequiredColNum() { return 1; }

  virtual arrow::Status Submit(ArrayList in, int max_group_id,
                               std::function<arrow::Status(int)> *on_valid,
                               std::function<arrow::Status()> *on_null);
  virtual arrow::Status Submit(std::vector<std::shared_ptr<arrow::Array>> in,
                               std::function<arrow::Status(uint64_t, uint64_t)> *on_valid,
                               std::function<arrow::Status()> *on_null);
  virtual arrow::Status Submit(const std::shared_ptr<arrow::Array> &in,
                               std::stringstream *ss,
                               std::function<arrow::Status(int)> *out);
  virtual arrow::Status Submit(const std::shared_ptr<arrow::Array> &in,
                               std::function<arrow::Status(uint32_t)> *on_valid,
                               std::function<arrow::Status()> *on_null);
  virtual arrow::Status Finish(ArrayList *out);
  virtual arrow::Status Finish(uint64_t offset, uint64_t length, ArrayList *out);
  virtual arrow::Status FinishAndReset(ArrayList *out);
  virtual uint64_t GetResultLength();
};

arrow::Status MakeUniqueAction(arrow::compute::FunctionContext* ctx,
                               std::shared_ptr<arrow::DataType> type,
                               std::shared_ptr<ActionBase>* out);

arrow::Status MakeCountAction(arrow::compute::FunctionContext* ctx,
                              std::shared_ptr<ActionBase>* out);

arrow::Status MakeCountLiteralAction(arrow::compute::FunctionContext* ctx, int arg,
                                     std::shared_ptr<ActionBase>* out);

arrow::Status MakeSumAction(arrow::compute::FunctionContext* ctx,
                            std::shared_ptr<arrow::DataType> type,
                            std::shared_ptr<ActionBase>* out);

arrow::Status MakeAvgAction(arrow::compute::FunctionContext* ctx,
                            std::shared_ptr<arrow::DataType> type,
                            std::shared_ptr<ActionBase>* out);

arrow::Status MakeMinAction(arrow::compute::FunctionContext* ctx,
                            std::shared_ptr<arrow::DataType> type,
                            std::shared_ptr<ActionBase>* out);

arrow::Status MakeMaxAction(arrow::compute::FunctionContext* ctx,
                            std::shared_ptr<arrow::DataType> type,
                            std::shared_ptr<ActionBase>* out);

arrow::Status MakeSumCountAction(arrow::compute::FunctionContext* ctx,
                                 std::shared_ptr<arrow::DataType> type,
                                 std::shared_ptr<ActionBase>* out);

arrow::Status MakeSumCountMergeAction(arrow::compute::FunctionContext* ctx,
                                      std::shared_ptr<arrow::DataType> type,
                                      std::shared_ptr<ActionBase>* out);

arrow::Status MakeAvgByCountAction(arrow::compute::FunctionContext* ctx,
                                   std::shared_ptr<arrow::DataType> type,
                                   std::shared_ptr<ActionBase>* out);

arrow::Status MakeStddevSampPartialAction(arrow::compute::FunctionContext* ctx,
                                          std::shared_ptr<arrow::DataType> type,
                                          std::shared_ptr<ActionBase>* out);

arrow::Status MakeStddevSampFinalAction(arrow::compute::FunctionContext* ctx,
                                        std::shared_ptr<arrow::DataType> type,
                                        std::shared_ptr<ActionBase>* out);
}
}
}
}