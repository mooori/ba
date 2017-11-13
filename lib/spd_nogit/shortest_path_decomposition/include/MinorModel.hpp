#pragma once

#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "Blob.hpp"
#include "Path.hpp"

class MinorModel {
    
public:
    typedef std::pair<Blob*, Blob*> BlobPointerPair;
    typedef std::vector<Blob*> BlobPVec;
    typedef std::pair<Path*, BlobPointerPair> MMEdge;
    typedef std::vector<MMEdge> MMEdges;
    
    MinorModel();
    
    MinorModel(MinorModel const &mm);
    
    MinorModel& operator=(MinorModel mm);
    
    ~MinorModel();
    
    void addPath(Path* p, Blob* b1, Blob* b2);
    
    void addBlob(Blob* b, Vertex_Descr v);
    
    BlobPVec::iterator removeBlob(Blob* b);
    
    MMEdges::iterator removePath(Path* p);
    
    void addUnconnectedBlobs(Blob* b1, Blob* b2);
    
    BlobPointerPair nextUnconnectedBlobs();
    
    bool hasUnconnectedBlobs();
    
    bool hasEdges();
    
    bool isFull();
    
    void print();
    
    bool test(bool debug);
    
    bool areBlobsConnected(Blob* b1, Blob* b2);
    
    int getT();
    
    BlobPVec blobs;
    MMEdges paths;
    
private:
    bool areBlobsInUnconnected(Blob* b1, Blob* b2);
    
    bool has_unconnected_blobs; //!isFull
    std::vector<BlobPointerPair> unconnected_blobs;
};