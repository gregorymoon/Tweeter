#include "StreamBuffer.h"

StreamBuffer::StreamBuffer(){
}

StreamBuffer::~StreamBuffer(){
}

Tweet* StreamBuffer::getToStream(){
	return toStream;
}

Tweet* StreamBuffer::getFromStream(){
	return fromStream;
}
