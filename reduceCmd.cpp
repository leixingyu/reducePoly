#include "reduceCmd.h"
#include <algorithm>
#include <stdio.h>

ReduceCmd::ReduceCmd() {
	mMesh = MGlobal::executeCommandStringResult("ls -selection");
};

ReduceCmd::~ReduceCmd() {};

void* ReduceCmd::creater() {
	return new ReduceCmd();
}

bool ReduceCmd::isUndoable() const {
	return false;
}

MSyntax ReduceCmd::newSyntax() {
	MSyntax syntax;

	syntax.addFlag("-p", "-percentage", MSyntax::kUnsigned);  // how to specify int value
	syntax.setObjectType(MSyntax::kSelectionList, 1, 1);
	syntax.useSelectionAsDefault(true);  // use selection as selection list

	syntax.enableEdit(false);
	syntax.enableQuery(false);

	return syntax;
}

MStatus ReduceCmd::getShapeNode(MDagPath &path) {
	if (path.apiType() == MFn::kMesh)
		return MS::kSuccess;

	unsigned int numShapes;
	path.numberOfShapesDirectlyBelow(numShapes);

	for (unsigned int i = 0; i < numShapes; i++) {
		path.extendToShapeDirectlyBelow(i);
		if (!path.hasFn(MFn::kMesh)) {
			path.pop();  // set path back to transform
			continue;
		}

		MFnDagNode fnNode(path);
		if (!fnNode.isIntermediateObject()) {
			return MS::kSuccess;
		}
		path.pop();
	}

	return MS::kFailure;  // didn't find shape node
}

MStatus ReduceCmd::doIt(const MArgList& argList) {
	// doIt gets all the arguments and store them as instances
	MArgDatabase argData(syntax(), argList); // syntax() is a built-in function to call our registered newSyntax()
	
	/* get selected objects dagPath*/
	MSelectionList selectedObj;
	argData.getObjects(selectedObj);
	selectedObj.getDagPath(0, m_basePath);

	getShapeNode(m_basePath); //gets the shape node under transform

	/* get flag */
	if (argData.isFlagSet("-p")) argData.getFlagArgument("-p", 0, m_percentage);
	else m_percentage = 70;

	MItMeshVertex itVert(m_basePath);
	m_count = itVert.count();
	unsigned int targetCount = round(m_count*m_percentage/100);  // get the desire poly-count

	unsigned int modifyVert, collapseVert;

	for (; m_count > targetCount; m_count--) {
		computeAllEdgeCosts();
		modifyVert = findMinCostVert();
		collapseVert = mCollapseVert[modifyVert];
		collapse(modifyVert, collapseVert);
		M3dView::active3dView().refresh(true, true);
	}

	return MS::kSuccess;
}

void ReduceCmd::collapse(unsigned int u, unsigned int v) {
	// move u to v
	MFnMesh fnMesh(m_basePath);
	MPoint vPos;
	fnMesh.getPoint(v, vPos);
	fnMesh.setPoint(u, vPos);

	// collapse u to v
	MGlobal::executeCommand("polyMergeVertex -d 0.01 -am 1 -cch 0 -ch 0 " + mMesh + ".vtx[" + u + "] " + mMesh + ".vtx[" + v + "] ");

	//fnMesh.updateSurface();
}

void ReduceCmd::computeAllEdgeCosts() {
	// iterate over input mesh
	MItMeshVertex itMesh(m_basePath);

	mLeastCost.clear();
	mCollapseVert.clear();

	MIntArray neighborIndices; // get nearby vertex
	int currentId;
	float leastCost = 1000.0f;
	int collapseVert = -1;
	float cost;

	while (!itMesh.isDone()) {
		currentId = itMesh.index();
		itMesh.getConnectedVertices(neighborIndices);
		for (unsigned int i = 0; i < neighborIndices.length(); i++) {
			cost = computeEdgeCost(currentId, neighborIndices[i]);
			if (cost < leastCost) {
				leastCost = cost;
				collapseVert = neighborIndices[i];
			}
		}
		mLeastCost.append(leastCost);  // store all the least cost of every vertex
		mCollapseVert.append(collapseVert);  // store the coresponding collapse destination vertex
		itMesh.next();
	}
}

float ReduceCmd::computeEdgeCost(int u, int v) {
	MItMeshVertex itMesh(m_basePath);
	MIntArray uConnectedFaces, vConnectedFaces;

	itMesh.setIndex(u, u);
	MPoint uPos = itMesh.position();
	itMesh.getConnectedFaces(uConnectedFaces);

	itMesh.setIndex(v, v);
	MPoint vPos = itMesh.position();
	itMesh.getConnectedFaces(vConnectedFaces);

	MVector edge = uPos - vPos;
	float edgeLength = edge.length();

	MIntArray sharedFaces;
	for (unsigned int i = 0; i < uConnectedFaces.length(); i++)
		if(itMesh.connectedToFace(uConnectedFaces[i]))
			sharedFaces.append(uConnectedFaces[i]);

	MItMeshPolygon itFace(m_basePath);
	MVector uNormal, vNormal;
	float curvature = 0;
	for (unsigned int i = 0; i < uConnectedFaces.length(); i++) {
		float mincurv = 1;
		itFace.getNormal(uConnectedFaces[i], uNormal);
		for (unsigned int j = 0; j < sharedFaces.length(); j++) {
			itFace.getNormal(sharedFaces[j], vNormal);
			float dotProd = uNormal * vNormal;
			curvature = std::min(mincurv, (1-dotProd)/2.0f);
		}
		curvature = std::max(curvature, mincurv);
	}
	return curvature * edgeLength;
}

unsigned int ReduceCmd::findMinCostVert() {
	// return the least cost vert index
	unsigned int leastIndex = 0;
	for (unsigned int i = 0; i < mLeastCost.length(); i++)
		if (mLeastCost[i] < mLeastCost[leastIndex])
			leastIndex = i;
	return leastIndex;
}

MStatus ReduceCmd::redoIt() {
	return MS::kSuccess;
}

MStatus ReduceCmd::undoIt() {
	// restore to initial state
	return MS::kSuccess;
}