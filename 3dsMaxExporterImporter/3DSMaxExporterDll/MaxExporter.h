#pragma once
#include <fstream>
#include <stack>
#include "SceneData.h"

typedef SceneData::TriangleBatch TriBatch;
typedef SceneData::Vertex Vertex;

// Cleans up a vector of ptrs
#define VECTOR_CLEANUP( vector ) {				\
	for( int i = 0; i < vector.size(); ++i )	\
	{											\
		delete vector[i];						\
	}											\
	vector.clear();								\
}

class MaxExporter : public SceneExport
{
public:
	MaxExporter();
	~MaxExporter();

	/** Exports the scene. */
	int				DoExport( const TCHAR* name, ExpInterface *ei, Interface* i, BOOL suppressprompts=FALSE, DWORD options=0 );

	/** Show DLL's "About..." box */
	void			ShowAbout( HWND hwnd );

	/** Number of extensions supported */
	int				ExtCount();
	
	/** Extension #n (i.e. "3DS") */
	const TCHAR*	Ext( int n );					
	
	/** Long ASCII description (i.e. "Autodesk 3D Studio File") */
	const TCHAR*	LongDesc();
	
	/** Short ASCII description (i.e. "3D Studio") */
	const TCHAR*	ShortDesc();
	
	/** ASCII Author name */
	const TCHAR*	AuthorName();
	
	/** ASCII Copyright message */
	const TCHAR*	CopyrightMessage();
	
	/** Other message #1 */
	const TCHAR*	OtherMessage1();
	
	/** Other message #2 */
	const TCHAR*	OtherMessage2();
	
	/** Version number * 100 (i.e. v3.01 = 301) */
	unsigned int	Version();
	
	/** Returns TRUE if option supported. */
	BOOL			SupportsOptions( int ext, DWORD options );

private:
	IGameScene*		m_igame;
	FILE*			m_fh;
	std::stack< std::string > m_errorMsgs;

	void			deinit();

	MaxExporter( const MaxExporter& );
	MaxExporter& operator=( const MaxExporter& );
	
	void collectNodes();
	void collectChildNodes( IGameNode* node );

	void loadObjects();
	void loadMesh( IGameNode* node, int index, int parentIndex );
	void loadStaticMesh( IGameNode* node, SceneData::StaticMesh* meshData );
	void loadSkelMesh( IGameNode* node, IGameSkin* skin,SceneData::SkelMesh* meshData );
	void loadMeshAnimationsData( IGameNode* node, SceneData::Mesh* meshData );
	void loadCamera( IGameNode* node, SceneData::Camera* cameraData );


	int getParentIndex( IGameNode* node );

	std::vector< IGameNode* > m_sceneNodes;
	std::vector< SceneData::Mesh > m_meshes;

	SceneData* m_sceneData;
	int m_currentObjId;
};