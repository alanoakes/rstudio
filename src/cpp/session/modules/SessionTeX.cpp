/*
 * SessionTeX.cpp
 *
 * Copyright (C) 2009-11 by RStudio, Inc.
 *
 * This program is licensed to you under the terms of version 3 of the
 * GNU Affero General Public License. This program is distributed WITHOUT
 * ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
 * AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
 *
 */

#include "SessionTeX.hpp"

#include <string>

#include <core/Log.hpp>
#include <core/Error.hpp>
#include <core/FilePath.hpp>
#include <core/Exec.hpp>
#include <core/json/JsonRpc.hpp>

#include <r/RExec.hpp>
#include <r/RRoutines.hpp>

#include <session/SessionModuleContext.hpp>

#include "tex/SessionCompilePdf.hpp"
#include "tex/SessionRnwWeave.hpp"
#include "tex/SessionTexEngine.hpp"
#include "tex/SessionPdfPreview.hpp"

using namespace core;

namespace session {
namespace modules { 
namespace tex {

namespace {




Error getTexCapabilities(const core::json::JsonRpcRequest& request,
                         json::JsonRpcResponse* pResponse)
{
   pResponse->setResult(tex::capabilitiesAsJson());
   return Success();
}

} // anonymous namespace


core::json::Array supportedRnwWeaveTypes()
{
   return tex::rnw_weave::supportedTypes();
}

json::Object capabilitiesAsJson()
{
   json::Object obj;

   obj["tex_installed"] = tex::engine::isInstalled();
   rnw_weave::getTypesInstalledStatus(&obj);

   return obj;
}

Error initialize()
{
   // install rpc methods
   using boost::bind;
   using namespace module_context;
   ExecBlock initBlock ;
   initBlock.addFunctions()
      (tex::compile_pdf::initialize)
      (tex::rnw_weave::initialize)
      (tex::engine::initialize)
      (tex::pdf_preview::initialize)
      (bind(registerRpcMethod, "get_tex_capabilities", getTexCapabilities))
      ;
  return initBlock.execute();
}

} // namespace tex
} // namespace modules
} // namesapce session

