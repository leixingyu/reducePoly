#pragma once
#ifndef REDUCECMD_H
#define REDUCECMD_H

#include <stdio.h>

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MPointArray.h>
#include <maya/MMeshIntersector.h>
#include <maya/MIntArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MGlobal.h>
#include <maya/M3dView.h>

#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MItDependencyGraph.h>

class ReduceCmd : public MPxCommand {
public:
	ReduceCmd();
	virtual ~ReduceCmd();
	static void* creater();

	virtual MStatus doIt(const MArgList& argList);
	virtual bool isUndoable() const;
	virtual MStatus redoIt();
	virtual MStatus undoIt();
	
	static MSyntax newSyntax();

	// user-defined function
	void computeAllEdgeCosts();
	float computeEdgeCost(int u, int v);
	void collapse(unsigned int u, unsigned int v);
	unsigned int findMinCostVert();

private:
	MStatus getShapeNode(MDagPath &path);

	MFloatArray mLeastCost;
	MIntArray mCollapseVert;
	MIntArray mEdgeFaces;
	MString mMesh;

	MDagPath m_basePath;
	unsigned int m_percentage;
	unsigned int m_count;
};

#endif // !ReduceCmd_H

