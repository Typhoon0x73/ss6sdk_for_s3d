#include "ssloader_ssqe.h"

namespace spritestudio6
{


SsSequencePack*	ssloader_ssqe::Load(const std::string& filename )
{

	SsSequencePack* sequence = new SsSequencePack();

	libXML::XMLDocument xml;
	if ( libXML::XML_SUCCESS == xml.LoadFile( filename.c_str() ) )
	{
		SsXmlIArchiver ar( xml.GetDocument() , "SpriteStudioSequencePack" );
		sequence->__Serialize( &ar );
	}else{
		delete sequence;
		sequence = 0;
	}

	return sequence;
}


SsSequence*	SsSequencePack::findSequence(SsString& seqname)
{

	for ( std::vector<SsSequence*>::iterator itr = sequenceList.begin()
		; itr != sequenceList.end() ; ++itr )
	{
		if ( (*itr)->name == seqname)
		{
			return (*itr);
		}
	}
	return 0;
}

}	// namespace spritestudio6
