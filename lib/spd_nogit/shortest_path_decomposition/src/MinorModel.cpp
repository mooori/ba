#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "../include/Blob.hpp"
#include "../include/Path.hpp"
#include "../include/Help.hpp"
#include "../include/MinorModel.hpp"


typedef std::pair<Blob*, Blob*> BlobPointerPair;
typedef std::vector<Blob*> BlobPVec;
typedef std::pair<Path*, BlobPointerPair> MMEdge;
typedef std::vector<MMEdge> MMEdges;

MinorModel::MinorModel() {
    has_unconnected_blobs = false;
}

MinorModel::MinorModel(MinorModel const &mm) {
    has_unconnected_blobs = mm.has_unconnected_blobs;
    
    std::map<Blob*, Blob*> map_v;
    for (BlobPVec::const_iterator it_blobs = mm.blobs.begin(); it_blobs != mm.blobs.end(); it_blobs++) {
        Blob* b = new Blob(*(*it_blobs));
        map_v[*it_blobs] = b;
        
        blobs.push_back(b);
    }
    
    for (BlobPVec::const_iterator it_blobs = blobs.begin(); it_blobs != blobs.end(); it_blobs++) {
        Blob::PebbleMap temp_pebbles;
        for (Blob::PebbleMap::iterator it_pebbles = (*it_blobs)->pebbles.begin(); it_pebbles != (*it_blobs)->pebbles.end(); it_pebbles++) {
            temp_pebbles[map_v[it_pebbles->first]] = it_pebbles->second;
        }
        (*it_blobs)->pebbles = temp_pebbles;
    }
    
    std::map<Path*, Path*> map_e;
    for (MMEdges::const_iterator it_paths = mm.paths.begin(); it_paths != mm.paths.end(); it_paths++) {
        Path* p = new Path(*(it_paths->first));
        map_e[(it_paths->first)] = p;
        BlobPointerPair blobs(std::make_pair(map_v[it_paths->second.first], map_v[it_paths->second.second]));
        MMEdge e(p, blobs);
        paths.push_back(e);
    }
    
    for (std::vector<BlobPointerPair>::const_iterator it = mm.unconnected_blobs.begin(); it != mm.unconnected_blobs.end(); it++) {
        BlobPointerPair blobs(std::make_pair(map_v[it->first], map_v[it->second]));
        unconnected_blobs.push_back(blobs);
    }
}

MinorModel& MinorModel::operator=(MinorModel mm) {
    has_unconnected_blobs = mm.has_unconnected_blobs;
    
    std::map<Blob*, Blob*> map_v;
    for (BlobPVec::const_iterator it_blobs = mm.blobs.begin(); it_blobs != mm.blobs.end(); it_blobs++) {
        Blob* b = new Blob(*(*it_blobs));
        map_v[*it_blobs] = b;
        
        blobs.push_back(b);
    }
    
    for (BlobPVec::const_iterator it_blobs = blobs.begin(); it_blobs != blobs.end(); it_blobs++) {
        Blob::PebbleMap temp_pebbles;
        for (Blob::PebbleMap::iterator it_pebbles = (*it_blobs)->pebbles.begin(); it_pebbles != (*it_blobs)->pebbles.end(); it_pebbles++) {
            temp_pebbles[map_v[it_pebbles->first]] = it_pebbles->second;
        }
        (*it_blobs)->pebbles = temp_pebbles;
    }
    
    std::map<Path*, Path*> map_e;
    for (MMEdges::const_iterator it_paths = mm.paths.begin(); it_paths != mm.paths.end(); it_paths++) {
        Path* p = new Path(*(it_paths->first));
        map_e[(it_paths->first)] = p;
        BlobPointerPair blobs(std::make_pair(map_v[it_paths->second.first], map_v[it_paths->second.second]));
        MMEdge e(p, blobs);
        paths.push_back(e);
    }
    
    for (std::vector<BlobPointerPair>::const_iterator it = mm.unconnected_blobs.begin(); it != mm.unconnected_blobs.end(); it++) {
        BlobPointerPair blobs(std::make_pair(map_v[it->first], map_v[it->second]));
        unconnected_blobs.push_back(blobs);
    }
    
    return *this;
}

MinorModel::~MinorModel() {
    for (BlobPVec::iterator it_blobs = blobs.begin(); it_blobs != blobs.end();) {
        delete (*it_blobs);
        it_blobs = blobs.erase(it_blobs);
    }
    
    for (MMEdges::iterator it_paths = paths.begin(); it_paths != paths.end();) {
        delete it_paths->first;
        it_paths = paths.erase(it_paths);
    }
}

void MinorModel::addPath(Path* p, Blob* b1, Blob* b2) { //no need to remove from unconnected, as calling of unconnected removes blobs by itself
    Vertex_Descr start = p->first();
    Vertex_Descr end = p->last();
    BlobPointerPair blobs(std::make_pair(b1, b2));
    MMEdge e(std::make_pair(p, blobs));
    paths.push_back(e);
    
    for (std::vector<BlobPointerPair>::iterator it_unconnected_blobs = unconnected_blobs.begin(); it_unconnected_blobs != unconnected_blobs.end(); it_unconnected_blobs++) {
        if ((it_unconnected_blobs->first == b1 && it_unconnected_blobs->second == b2) || (it_unconnected_blobs->first == b2 && it_unconnected_blobs->second == b1)) {
            unconnected_blobs.erase(it_unconnected_blobs);
            break;
        }
    }
    
    if (unconnected_blobs.empty()) { //
        has_unconnected_blobs = false;
    }
    
    if (b1->hasVertex(start) && b2->hasVertex(end)) {
        b1->setPebble(b2, start);
        b2->setPebble(b1, end);
    }
    
    else if (b1->hasVertex(end) && b2->hasVertex(start)) {
        b2->setPebble(b1, start);
        b1->setPebble(b2, end);
    }
    
    else {
        std::cout << "blobs " << b1->getID() << " and " << b2->getID() << " dont have vertices " << *((int*) &start) << " and " << *((int*) &end) << std::endl;
        std::cin.get();
        exit(-1);
    }
}

void MinorModel::addBlob(Blob* b, Vertex_Descr v) { //also adds unconnected blobs and initializes pebbles
    b->addConnectedVertex(v);
    
    for (BlobPVec::iterator it_blobs = blobs.begin(); it_blobs != blobs.end(); it_blobs++) {
        Blob* curr_blob = (*it_blobs);
        Blob* new_blob = b;
        
        BlobPointerPair blob_pair(std::make_pair(curr_blob, new_blob));
        unconnected_blobs.push_back(blob_pair);
        has_unconnected_blobs = true;
        
        new_blob->setPebble(curr_blob, v);
        curr_blob->setPebble(new_blob, Help::null_vertex);
    }
    
    
    blobs.push_back(b);
}

BlobPVec::iterator MinorModel::removeBlob(Blob* b) {
    BlobPVec::iterator ret;
    
    ret = blobs.erase(std::find(blobs.begin(), blobs.end(), b));
    
    //Delete from unconnected
    for (std::vector<BlobPointerPair>::iterator it_unconnected_blobs = unconnected_blobs.begin(); it_unconnected_blobs != unconnected_blobs.end();) {
        if (it_unconnected_blobs->first == b || it_unconnected_blobs->second == b) {
            it_unconnected_blobs = unconnected_blobs.erase(it_unconnected_blobs);
        }
        else {
            it_unconnected_blobs++;
        }
    }
    
    if (unconnected_blobs.empty())
        has_unconnected_blobs = false;
    
    //Delete from paths
    Blob *b1, *b2;
    for (MMEdges::iterator it_paths = paths.begin(); it_paths != paths.end();) {
        b1 = it_paths->second.first;
        b2 = it_paths->second.second;
        Path* p = it_paths->first;
        if (b1 == b || b2 == b) {
            //decrement connections of remaining blob
            it_paths = paths.erase(it_paths);
            delete p;
        }
        else {
            it_paths++;
        }
    }
    
    delete b;
    return ret;
}

MMEdges::iterator MinorModel::removePath(Path* p) {
    MMEdges::iterator ret = paths.begin();
    
    if (!has_unconnected_blobs) {
        has_unconnected_blobs = true;
    }
    
    for (MMEdges::iterator it_paths = paths.begin(); it_paths != paths.end();) {
        if (it_paths->first == p) {
            Blob* b1 = it_paths->second.first;
            Blob* b2 = it_paths->second.second;
            
            unconnected_blobs.push_back(BlobPointerPair(b1, b2));
            has_unconnected_blobs = true;
            it_paths = paths.erase(it_paths);
            delete p;
            return it_paths;
        }
        else {
            it_paths++;
        }
    }
    return ret;
}

void MinorModel::addUnconnectedBlobs(Blob* b1, Blob* b2) {
    unconnected_blobs.push_back(BlobPointerPair(b1, b2));
    has_unconnected_blobs = true;
}

BlobPointerPair MinorModel::nextUnconnectedBlobs() {
    unsigned int pos = Help::random_int(unconnected_blobs.size());
    BlobPointerPair ret = unconnected_blobs[pos];
    return ret;
}

bool MinorModel::hasUnconnectedBlobs() {
    return has_unconnected_blobs;
}

bool MinorModel::hasEdges() {
    return !paths.empty();
}

bool MinorModel::isFull() {
    return !has_unconnected_blobs;
}

void MinorModel::print() {
    Help::output("--------MinorModel-------");
    Help::output(this);
    Help::output();
    std::cout << "t: ";
    Help::output(getT());
    
    Help::output("blobs:");
    for (BlobPVec::const_iterator it_blobs = blobs.begin(); it_blobs != blobs.end(); it_blobs++) {
        (*it_blobs)->print();
    }
    Help::output();
    Help::output("paths:");
    for (MMEdges::const_iterator it_paths = paths.begin(); it_paths != paths.end(); it_paths++) {
        std::cout << it_paths->second.first->getID() << " <-> " << it_paths->second.second->getID() << std::endl;
        it_paths->first->print();
        Help::output();
    }
    Help::output();
    Help::output("unconnected blobs:");
    for (std::vector<BlobPointerPair>::iterator it_unconnected_blobs = unconnected_blobs.begin(); it_unconnected_blobs != unconnected_blobs.end(); it_unconnected_blobs++) {
        std::cout << it_unconnected_blobs->first->getID() << " - " << it_unconnected_blobs->second->getID() << std::endl;
    }
    Help::output();
}

bool MinorModel::test(bool debug) {
    bool ret = true;
    if (debug)
        Help::output("Tests of minorModel...");
    
    //unconnected <-> nopath in edges
    bool test = true;
    for (BlobPVec::const_iterator it_blobs = blobs.begin(); it_blobs != blobs.end(); it_blobs++) {
        for (BlobPVec::const_iterator it_blobs_2 = blobs.begin(); it_blobs_2 != blobs.end(); it_blobs_2++) {
            if (it_blobs_2 != it_blobs) {
                if ((areBlobsConnected(*it_blobs, *it_blobs_2) && areBlobsInUnconnected(*it_blobs, *it_blobs_2))
                    || (!areBlobsConnected(*it_blobs, *it_blobs_2) && !areBlobsInUnconnected(*it_blobs, *it_blobs_2))) {
                    test = false;
                ret = false;
                if (debug)
                    std::cout << (*it_blobs)->getID() << " and " << (*it_blobs_2)->getID() << " failed" << std::endl;
                    }
            }
        }
    }
    
    if (debug) {
        Help::output("Test 1: No Path between b1 and b2 <-> b1 and b2 in unconnected...");
        if (test)
            Help::output("OK!");
        else
            Help::output("ERROR");
    }
    
    //every blob has #blobs-1 pebbles
    test = true;
    for (BlobPVec::iterator it_blobs = blobs.begin(); it_blobs != blobs.end(); it_blobs++) {
        if ((*it_blobs)->nPebbles() != blobs.size() - 1) {
            test = false;
            ret = false;
            if (debug)
                std::cout << "MinorModel has " << blobs.size() << " vertices but Blob " << (*it_blobs)->getID() << " has " << (*it_blobs)->nPebbles() << " pebbles" << std::endl;
        }
    }
    
    if (debug) {
        Help::output("Test 2: Number of pebbles...");
        if (test)
            Help::output("OK!");
        else
            Help::output("ERROR");
    }
    
    //every pebble lies on connected vertex
    test = true;
    for (BlobPVec::iterator it_blobs = blobs.begin(); it_blobs != blobs.end(); it_blobs++) {
        for (BlobPVec::iterator it_blobs_2 = blobs.begin(); it_blobs_2 != blobs.end(); it_blobs_2++) {
            if (it_blobs_2 != it_blobs) {
                if (!(*it_blobs)->hasVertex((*it_blobs)->getPebble(*it_blobs_2))) {
                    test = false;
                    ret = false;
                    if (debug)
                        std::cout << "In " << (*it_blobs)->getID() << ": Pebble to " << (*it_blobs_2)->getID() << " is unconnected" << std::endl;
                }
            }
        }
    }
    if (debug) {
        Help::output("Test 3: pebbles lie on connected vertices...");
        if (test)
            Help::output("OK!");
        else
            Help::output("ERROR");
    }
    
    //paths have their endpoints in corresponding blobs
    test = true;
    for (MMEdges::iterator it_paths = paths.begin(); it_paths != paths.end(); it_paths++) {
        Vertex_Descr start = it_paths->first->first();
        Vertex_Descr end = it_paths->first->last();
        Blob* b1 = it_paths->second.first;
        Blob* b2 = it_paths->second.second;
        
        if (!((b1->hasVertex(start) && b2->hasVertex(end)) || (b1->hasVertex(end) && b2->hasVertex(start)))) {
            test = false;
            ret = false;
            if (debug) {
                std::cout << "Path ";
                it_paths->first->print();
                std::cout << "doesn't connect " << b1->getID() << " and " << b2->getID() << std::endl;
            }
        }
    }
    
    if (debug) {
        Help::output("Test 4: endpoints of paths are in corresponding blobs...");
        if (test)
            Help::output("OK!");
        else
            Help::output("ERROR");
    }
    
    return ret;
}

bool MinorModel::areBlobsConnected(Blob* b1, Blob* b2) {
    for (MMEdges::iterator it_paths = paths.begin(); it_paths != paths.end(); it_paths++) {
        if ((it_paths->second.first == b1 && it_paths->second.second == b2) || (it_paths->second.first == b2 && it_paths->second.second == b1)) {
            return true;
        }
    }
    return false;
}

int MinorModel::getT() {
    if (has_unconnected_blobs)
        return blobs.size() - 1;
    else
        return blobs.size();
}

bool MinorModel::areBlobsInUnconnected(Blob* b1, Blob* b2) {
    for (std::vector<BlobPointerPair>::iterator it_unconnected_blobs = unconnected_blobs.begin(); it_unconnected_blobs != unconnected_blobs.end(); it_unconnected_blobs++) {
        if ((it_unconnected_blobs->first == b1 && it_unconnected_blobs->second == b2)
            || (it_unconnected_blobs->first == b2 && it_unconnected_blobs->second == b1))
            return true;
    }
    return false;
}

