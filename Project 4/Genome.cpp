#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <cctype>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    
    string m_name;
    string seq;
    
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
    m_name = nm;
    seq = sequence;
    // This compiles, but may not be correct
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
    string nextLine;
    
    string name;
    
    string sequence;
    
    while(getline(genomeSource, nextLine)) {
        if(nextLine == "") return false;
        if(nextLine[0] == '>') {
            
            if(name != "") {
            
                for(int i = 0; i < sequence.length(); i++) {
                    toupper(sequence[i]);
                    if(sequence[i] != 'A' && sequence[i] != 'T' && sequence[i] != 'C' && sequence[i] != 'G' && sequence[i] != 'N') return false;
                }
                genomes.push_back(Genome(name, sequence));
                sequence = "";
            }
            name = nextLine.substr(1);
        }
        else {
            if(name == "") return false;
            sequence += nextLine;
        }
    }
    for(int i = 0; i < sequence.length(); i++) {
        toupper(sequence[i]);
        if(sequence[i] != 'A' && sequence[i] != 'T' && sequence[i] != 'C' && sequence[i] != 'G' && sequence[i] != 'N') return false;
    }
    genomes.push_back(Genome(name, sequence));
    
    return true;  // This compiles, but may not be correct
}

int GenomeImpl::length() const
{
    return seq.length();  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
    return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    if(position + length > seq.length()) return false;
    fragment = seq.substr(position, length);
    return true;  // This compiles, but may not be correct
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
