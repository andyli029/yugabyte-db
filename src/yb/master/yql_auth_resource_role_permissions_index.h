// Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//

#ifndef YB_MASTER_YQL_AUTH_RESOURCE_ROLE_PERMISSIONS_INDEX_H
#define YB_MASTER_YQL_AUTH_RESOURCE_ROLE_PERMISSIONS_INDEX_H

#include "yb/master/yql_virtual_table.h"

namespace yb {
namespace master {

// VTable implementation of system_auth.resource_role_permissions_index
class YQLAuthResourceRolePermissionsIndexVTable : public YQLVirtualTable {
 public:
  explicit YQLAuthResourceRolePermissionsIndexVTable(const Master* const master);

  CHECKED_STATUS RetrieveData(const QLReadRequestPB& request,
                              std::unique_ptr<QLRowBlock>* vtable) const;

 protected:
  Schema CreateSchema() const;

 private:
  static constexpr const char* const kResource = "resource";
  static constexpr const char* const kRole = "role";
};

}  // namespace master
}  // namespace yb
#endif // YB_MASTER_YQL_AUTH_RESOURCE_ROLE_PERMISSIONS_INDEX_H
