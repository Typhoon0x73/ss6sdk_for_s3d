#pragma once
#ifndef __SSPLAYER_MESH__
#define __SSPLAYER_MESH__

#include <Siv3D.hpp>
#include <memory>

namespace spritestudio6
{
class ISSTexture;

enum
{
	SSMESHPART_BONEMAX = 128,
	SSMESHPART_CHECKRANGE = 4,
};
struct StBoneWeight
{
	int		   		weight[SSMESHPART_BONEMAX];
	float		   	weight_f[SSMESHPART_BONEMAX];

	SsPartState*    bone[SSMESHPART_BONEMAX];
	SsVector3		offset[SSMESHPART_BONEMAX];

	int				bindBoneNum;

	float			length[SSMESHPART_BONEMAX];    //temp
	float			lengthtotal;  				   //temp

	int	getBoneNum() { return bindBoneNum; }

	void Cleanup()
	{
		bindBoneNum = 0;
		lengthtotal = 0.0f;

		for(int i=0; i<SSMESHPART_BONEMAX; i++)
		{
			weight[i] = 0;
			weight_f[i] = 0.0f;

			bone[i] = nullptr;
			offset[i] = SsVector3(0, 0, 0);

			length[i] = 0.0f;
		}
	}
};

class SsMeshPart
{
public:

	std::unique_ptr<Buffer2D> buffer2D;
	std::unique_ptr<std::vector<float>>				vertices;		//[3 * 10];///< 座標
	//std::unique_ptr<std::vector<float>>				colors;			//[4 * 10];	///< カラー
	//std::unique_ptr<std::vector<float>>				weightColors;	//[4 * 10];	///< ウェイト色分けカラー
	//std::unique_ptr<std::vector<float>>				uvs;			//[2 * 10];		///< UV
	//std::unique_ptr<std::vector<unsigned short>>	indices;
	int				indices_num;
	int				tri_size;
	int				ver_size;
	//std::unique_ptr<std::vector<float>>	draw_vertices;	//[3 * 10];///< 座標

	std::unique_ptr<std::vector<float>>	offset_world_vertices;	// 描画に使われるデフォームアトリビュート
											
	//ツール用テンポラリワーク [editer]
	std::unique_ptr<std::vector<SsVector2>>	vertices_outer;
	std::unique_ptr<std::vector<SsVector2>>	update_vertices_outer;
	size_t						outter_vertexnum;

public:
	std::unique_ptr<std::vector<StBoneWeight>>	bindBoneInfo;

	SsCell*  			targetCell;
	Texture				targetTexture;
	SsPartState*	   	myPartState;

	//テンポラリ [editor]
	bool				isBind;


public:
	SsMeshPart() :
		isBind(false), targetCell(0)
		, bindBoneInfo()
		, buffer2D( nullptr )
		//, weightColors()
		, vertices_outer()
		//, draw_vertices()
		, myPartState(0)
		, vertices()//, colors(), uvs(), indices()
		, update_vertices_outer()
		, offset_world_vertices()
	{
	}


	SsMeshPart(SsPartState* s) :
		isBind(false), targetCell(0)
		, buffer2D( nullptr )
		//, weightColors()
		, bindBoneInfo()
		, vertices_outer()
		//, draw_vertices()
		, vertices()//, colors(), uvs(), indices()
		, update_vertices_outer()
		, offset_world_vertices()
	{
		myPartState = s;
	}

	~SsMeshPart()
	{
		Cleanup();
	}

	void	Cleanup();
	void	makeMesh();

	int		getVertexNum() { return ver_size; }
	StBoneWeight*	getVerticesWeightInfo(int index) {
		if (index > getVertexNum())return 0;
		std::vector<StBoneWeight>& bindBoneInfoRaw = *(bindBoneInfo.get());
		return &bindBoneInfoRaw[index];
	}

	void    updateTransformMesh();            //再生時用　（バインドされたボーンに沿って変形を行う）

	//デフォーム関連
	SsVector3 getOffsetWorldVerticesFromKey(int index);
	void	setOffsetWorldVertices(int index, const SsVector3 & v);
	SsVector2 getOffsetLocalVertices(int index);

/*
	void	renderVertex();
	void	renderMesh(float alpha, bool renderTexture);
	void	renderBoneWeightColor(float alpha, bool renderTexture);
	void	update_matrix(float * matrix);  //バインド前（セットアップモード用のマトリクスアップデート)

	void		calcVerticesPos(SsAnimeState* state);
	SsVector3   getWorldVertexPoint(int index) { return calc_world_vertices[index]; }
*/
//	Editer用
//	void    verticesWeightColorCalc();
//	void	draw_world_vertices();
//	void	draw_world_vertices_once(int index, SsFColor c);
//	int 	isTouchVertex(float mx, float my);
//	bool  	isInPoint(float x, float y);
//	void    bindBoneSmoth(std::vector<SsPartState*>& list);

};



class SsPart;
class SsMeshPart;
//class SsAnimeState;

class   SsMeshAnimator
{
private:
	void	modelLoad();

public:
	SsAnimeDecoder* bindAnime;

	std::vector<SsPartState*>    	meshList;
	std::vector<SsPartState*>    	animeboneList;
	std::vector<SsPartState*>    	jointList;

public:
	SsMeshAnimator();
	virtual ~SsMeshAnimator() {}

	void	setAnimeDecoder(SsAnimeDecoder* s);

	void	update();
	void	makeMeshBoneList();
	void	copyToSsMeshPart(SsMeshBind* src, SsMeshPart* dst, std::map<int, SsPartState*> boneIdxList);


};



}	// namespace spritestudio6

#endif
