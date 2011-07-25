/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
 *
 * Copyright (C) 2011  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "testmodule.h"
#include "examplemodule.h"
#include "emdata.h"
#include "marker.h"
#include "cellularautomata.h"
#include "copyrastermap.h"
#include "randommarks.h"
#include "randompoints.h"
#include "exportrasterdata.h"
#include "simpleagentmodel.h"
#include "createemptyrastermap.h"
#include "importrasterdata.h"
#include "importdxf.h"
#include "linestofaces.h"
#include "markcell.h"
#include "vectordataaddition.h"
#include "singlevalue.h"
#include "loopgroup.h"
#include "group.h"
#include <iostream>
#include "nodefactory.h"
#include "moduleregistry.h"
#include "viewgriddata.h"
#include "rasterdataaddition.h"
#include "blocks.h"
#include "appendattributes.h"
#include "plotdoubledata.h"
#include "sumoverrasterdata.h"
#include "exportdoublevalue.h"
#include "writedatatofile.h"
#include "writeattributestofile.h"
#include "importxyzasshape.h"
#include "wassertirolsampler.h"
#include "offsetvectordata.h"
#include "appendstringattribute.h"
#include "viewvectordata.h"
#include "calculatecentroid.h"
#include "appendfaceidtoface.h"
#include "joinpointdatatofaces.h"
using namespace std;

/**
  * @addtogroup Modules
  * @brief Bacis VIBe Modules
  * @author Christian Urich
  */

extern "C" void VIBE_HELPER_DLL_EXPORT  registerModules(ModuleRegistry *registry) {
    registry->addNodeFactory(new NodeFactory<MarkCell>());
    registry->addNodeFactory(new NodeFactory<TestModule>());
    //    registry->addNodeFactory(new NodeFactory<ExampleModule>());
    //    registry->addNodeFactory(new NodeFactory<EMData>());
    registry->addNodeFactory(new NodeFactory<Marker>());
    //    registry->addNodeFactory(new NodeFactory<ExampleModule>());
    registry->addNodeFactory(new NodeFactory<CellularAutomata>());
    //    registry->addNodeFactory(new NodeFactory<CopyRasterMap>());
    registry->addNodeFactory(new NodeFactory<RandomPoints>());
    //    registry->addNodeFactory(new NodeFactory<RandomMarks>());
    registry->addNodeFactory(new NodeFactory<ExportRasterData>());
    //    registry->addNodeFactory(new NodeFactory<SimpleAgentModel>());
    registry->addNodeFactory(new NodeFactory<CreateEmptyRasterMap>());
    registry->addNodeFactory(new NodeFactory<ImportRasterData>());
    registry->addNodeFactory(new NodeFactory<ImportDXF>());
    registry->addNodeFactory(new NodeFactory<LinesToFaces>());

    //    registry->addNodeFactory(new NodeFactory<VectorDataAddition>());
    registry->addNodeFactory(new NodeFactory<SingleValue>());
    registry->addNodeFactory(new NodeFactory<LoopGroup>());
    registry->addNodeFactory(new NodeFactory<ViewGridData>());
    registry->addNodeFactory(new NodeFactory<RasterDataAddition>());
    registry->addNodeFactory(new NodeFactory<Blocks>());
    registry->addNodeFactory(new NodeFactory<AppendAttributes>());
    registry->addNodeFactory(new NodeFactory<PlotDoubleData>());
    registry->addNodeFactory(new NodeFactory<SumOverRasterData>());
    registry->addNodeFactory(new NodeFactory<ExportDoubleValue>());
    registry->addNodeFactory(new NodeFactory<WriteDataToFile>());
    registry->addNodeFactory(new NodeFactory<WriteAttributesToFile>());
    registry->addNodeFactory(new NodeFactory<ImportXYZAsShape>());
    registry->addNodeFactory(new NodeFactory<WassertirolSampler>());
    registry->addNodeFactory(new NodeFactory<OffsetVectorData>());
    registry->addNodeFactory(new NodeFactory<AppendStringAttribute>());
    registry->addNodeFactory(new NodeFactory<ViewVectorData> ());
    registry->addNodeFactory(new NodeFactory<CalculateCentroid>());
    registry->addNodeFactory(new NodeFactory<AppendFaceIDToFace>());
    registry->addNodeFactory(new NodeFactory<JoinPointDataToFaces>());

}

