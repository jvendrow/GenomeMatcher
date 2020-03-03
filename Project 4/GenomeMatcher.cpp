#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Trie.h"
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    
    int minLength;
    
    Trie<DNAMatch> tree;
    
    vector<Genome> genomes;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    minLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    genomes.push_back(genome);
    for(int i = 0; i < genome.length() - minLength; i++) {
        string frag;
        genome.extract(i, minLength, frag);
        DNAMatch a;
        a.position = i;
        a.length = minLength;
        a.genomeName = genome.name();
        tree.insert(frag, a);
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return minLength;  // This compiles, but may not be correct
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    if(minimumLength < minLength) return false;
    
    if(fragment.length() < minLength) return false;
    
    string toSearch = fragment.substr(0, minLength);
    
    vector<DNAMatch> found = tree.find(toSearch, exactMatchOnly);
    
    unordered_map<string, const Genome*> map;
    
    for(int i = 0; i < genomes.size(); i++) {
        map[genomes[i].name()] = &(genomes[i]);
    }

    matches.clear();
    
    for(int i = 0; i < found.size(); i++) {
        
        const Genome* a = map[found[i].genomeName];
        
        int pos = found[i].position;
        
        string letter;
        
        int length = minLength;

        a->extract(pos + length, 1, letter);
        
        while(a->length() > pos + length && length < fragment.length() && letter == fragment.substr(length, 1)) {
            length ++;
            a->extract(pos + length, 1, letter);
        }
        
        if(length < minimumLength) {
            continue;
        }
        
        found[i].length = length;
        matches.push_back(found[i]);
    }
    
    if(matches.size() == 0) return false;
    
    return true;

}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    
    if(fragmentMatchLength < minLength) return false;
    
    for(int i = 0; i < query.length() - fragmentMatchLength; i += fragmentMatchLength) {
        
        string next;
        query.extract(i, fragmentMatchLength, next);
        
        vector<DNAMatch> matches;
        findGenomesWithThisDNA(next, fragmentMatchLength, exactMatchOnly, matches);
        
        unordered_map<string, int> map;
        
        
        for(int i = 0; i < matches.size(); i++) {
            
            if(map.find(matches[i].genomeName) == map.end()) {
                map[matches[i].genomeName] = 1;
            }
            else {
                map[matches[i].genomeName] ++;
            }
        }
        
        for(auto itr = map.begin(); itr != map.end(); itr++) {
            double percentage = 100 * (itr->second) / (double)(query.length() / fragmentMatchLength);
            
            GenomeMatch a;
            a.genomeName = itr->first;
            a.percentMatch = percentage;
            results.push_back(a);
            
        }
    }
    if(results.size() == 0) return false;
    return true;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
