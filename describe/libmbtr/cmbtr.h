#ifndef CMBTR_H
#define CMBTR_H

using namespace std;

/**
 * Represents a combination of two integer indices: i and j.
 */
struct index2d {
    int i;
    int j;

    // These comparison operators are needed in order to use this struct as map
    // key
    bool operator==(const index2d &o) const {
        return (i == o.i) && (j == o.j);
    }
    bool operator<(const index2d &o) const {
        return (i < o.i) || (i == o.i && j < o.j);
    }
};

/**
 * Represents a combination of three integer indices: i, j and k.
 */
struct index3d {
    int i;
    int j;
    int k;

    // These comparison operators are needed in order to use this struct as map
    // key
    bool operator==(const index3d &o) const {
        return (i == o.i) && (j == o.j) & (k == o.k);
    }
    bool operator<(const index3d &o) const {
        return (i < o.i) || (i == o.i && j < o.j) || (i == o.i && j == o.j && k < o.k);
    }
};

/**
 * Implementation for the performance-critical parts of MBTR.
 */
class CMBTR {

    public:
        /**
         * Default constructor
         */
        CMBTR() {};

        /**
         * Constructor
         *
         * @param positions Atomic positions in cartesian coordinates.
         * @param atomicNumbers Atomic numbers.
         * @param atomicNumberToIndexMap Mapping between atomic numbers and
         * their position in the final MBTR vector.
         * @param cellLimit The number of atoms in the original cell. The atoms
         * from 0-cellLimit belong to the original simulation cell, rest are
         * periodic copies.
         */
        CMBTR(vector<vector<float> > positions, vector<int> atomicNumbers, map<int,int> atomicNumberToIndexMap, int cellLimit);

        /**
         * Returns a list of 2D indices for the atom combinations that need to
         * be calculated for the k=2 term.
         *
         * @return A list of 2D indices.
         */
        vector<index2d> getk2Indices();

        /**
         * Returns a list of 3D indices for the atom combinations that need to
         * be calculated for the k=3 term.
         *
         * @return A list of 3D indices for k3.
         */
        vector<index3d> getk3Indices();

        /**
         * Weighting of 1 for all indices. Usually used for finite small
         * systems.
         *
         * @return
         */
        map<index2d, float> k2WeightUnity(const vector<index2d> &indexList);

        /**
         * Weighting of 1 for all indices. Usually used for finite small
         * systems.
         *
         * @return
         */
        map<index3d, float> k3WeightUnity(const vector<index3d> &indexList);

        /**
         * Weighting defined as e^(-sx), where x is the distance from
         * A->B and s is the scaling factor.
         *
         * @param indexList List of pairs of atomic indices.
         * @param scale The prefactor in the exponential weighting.
         * @param cutoff Minimum value of the weighting to consider.
         * @return Map of distances for pairs of atomic indices.
         */
        map<index2d, float> k2WeightExponential(const vector<index2d> &indexList, float scale, float cutoff);

        /**
         * Weighting defined as e^(-sx), where x is the distance from
         * A->B->C->A and s is the scaling factor.
         *
         * @param indexList List of triplets of atomic indices.
         * @param scale The prefactor in the exponential weighting.
         * @param cutoff Minimum value of the weighting to consider.
         * @return Map of distances for triplets of atomic indices.
         */
        map<index3d, float> k3WeightExponential(const vector<index3d> &indexList, float scale, float cutoff);

        /**
         * Calculates the inverse distance geometry function defined for k=2.
         *
         * @return A map of inverse distance values for the given atomic pairs.
         */
        map<index2d, float> k2GeomInverseDistance(const vector<index2d> &indexList);

        /**
         * Calculates the cosine geometry function defined for k3.
         *
         * @return The cosine value for the angle defined between the given
         * three atomic indices.
         */
        map<index3d, float> k3GeomCosine(const vector<index3d> &indexList);

        /**
         * Calculates a 3D matrix of distance vectors between atomic positions.
         *
         * @return A 3D matrix of displacement vectors. First index is the
         * index of ith atom, second index is the index of jth atom, third
         * index is the cartesian component.
         */
        vector<vector<vector<float> > > getDisplacementTensor();

        /**
         * Calculates a 2D matrix of distances between atomic positions.
         *
         * @return A 2D matrix of distances. First index is the
         * index of ith atom, second index is the index of jth atom.
         */
        vector<vector<float> > getDistanceMatrix();

        /**
         * Calculates a 2D matrix of inverse distances between atomic
         * positions.
         *
         * @return A 2D matrix of inverse distances. First index is the
         * index of ith atom, second index is the index of jth atom.
         */
        //vector<vector<float> > getInverseDistanceMatrix();

        /**
         * Calculates a mapping of inverse distances between two different
         * atomic elements.
         *
         * @return A map where the key is a pair of atomic numbers and the
         * value is a list of inverse distances corresponding to pairs of atoms
         * with the given atomic numbers.
         */
        //map<pair<int,int>, vector<float> > getInverseDistanceMap();

        /**
         * Calculates a list of values for the k=2 geometry functions and the
         * corresponding weights for each pair of atomic numbers.
         *
         * @return A pair of maps for the geometry- and weighting function
         * values for each pair of atomic elements.
         */
        pair<map<index2d,vector<float> >, map<index2d,vector<float> > > getK2Map(string geomFunc, string weightFunc, map<string, float> parameters=map<string, float>());

        /**
         * Calculates a list of values for the k=3 geometry functions and the
         * corresponding weights for each triplet of atomic numbers.
         *
         * @return A pair of maps for the geometry- and weighting function
         * values for each triplet of atomic elements.
         */
        pair<map<index3d,vector<float> >, map<index3d,vector<float> > > getK3Map(string geomFunc, string weightFunc, map<string, float> parameters=map<string, float>());

        /**
         * A convenience function that provides a Cython-compatible interface
         * to the getK2Map-function. Cython cannot handle custom map keys, so a
         * string key is provided by this function.
         */
        pair<map<string,vector<float> >, map<string,vector<float> > > getK2MapCython(string geomFunc, string weightFunc, map<string, float> parameters=map<string, float>());

        /**
         * A convenience function that provides a Cython-compatible interface
         * to the getK3Map-function. Cython cannot handle custom map keys, so a
         * string key is provided by this function.
         */
        pair<map<string,vector<float> >, map<string,vector<float> > > getK3MapCython(string geomFunc, string weightFunc, map<string, float> parameters=map<string, float>());

    private:
        vector<vector<float> > positions;
        vector<int> atomicNumbers;
        map<int,int> atomicNumberToIndexMap;
        int cellLimit;
        vector<vector<vector<float> > > displacementTensor;
        bool displacementTensorInitialized;
        vector<index2d> k2Indices;
        bool k2IndicesInitialized;
        vector<index3d> k3Indices;
        bool k3IndicesInitialized;
        pair<map<index2d, vector<float> >, map<index2d, vector<float> > > k2Map;
        bool k2MapInitialized;
        pair<map<index3d, vector<float> >, map<index3d, vector<float> > > k3Map;
        bool k3MapInitialized;
};

#endif
