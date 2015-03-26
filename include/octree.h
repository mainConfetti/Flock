#ifndef OCTREE_H
#define OCTREE_H

#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <vector>
#include <stdlib.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file octree.h
/// @author Alexander la Tourelle
/// @version 1.0
/// @date 22/03/15
/// @class NGLScene
/// @brief A simple octree to be used for finding the neighbours of a boid in a flocking system
//----------------------------------------------------------------------------------------------------------------------
class Octree
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor
    /// @param [in] _origin the physical centre of the node
    /// @param [in] _halfD half the width, depth, or height of the node
    /// @param [in] _height the height of the octree
    //----------------------------------------------------------------------------------------------------------------------
    Octree(ngl::Vec3 _origin, ngl::Vec3 _halfD, int _height);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor recursivley destroys octants
    //----------------------------------------------------------------------------------------------------------------------
    ~Octree();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the m_data member variable
    /// @param [in] _data the data to assign to m_data
    //----------------------------------------------------------------------------------------------------------------------
    void setData(ngl::Vec4 _data);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Determine which octant of the tree contains "point"
    /// @param [in] point the point to check
    /// @return int denoting the octant containing "point"
    //----------------------------------------------------------------------------------------------------------------------
    int getOctantContainingPoint(ngl::Vec3 point);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief checks whether the current node is a leaf node
    /// @return true if node is leaf or false if not
    //----------------------------------------------------------------------------------------------------------------------
    bool isLeaf();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief if this node doesnt have a data point; insert one or if
    /// is does; split this node into 8 children and insert old and new data points
    /// @param [in] _data the data to insert
    //----------------------------------------------------------------------------------------------------------------------
    void insert(ngl::Vec4 _data);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief query the tree for points within a bounding sphere
    /// @param [in] centre the centre of the bounding sphere
    /// @param [in] radius the radius of the bounding sphere
    /// @param [in] results an array of the points inside the bounding sphere
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec4 *getPointsInsideSphere(ngl::Vec3 centre, float radius);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief array for storing the results of getPointsInsideSphere
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Vec4 *> m_results;
    void findData(ngl::Vec3 data);
    void clearResults();
    void clearTree();
    int m_plusIndex=0;
    std::vector<ngl::Vec4 *> temp_data;
    void cleanResults();
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the physical centre of this node
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_origin;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief half the width, depth or height of this node
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_halfD;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the height of the current node
    //----------------------------------------------------------------------------------------------------------------------
    int m_height;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pointers to child octants
    //----------------------------------------------------------------------------------------------------------------------
    Octree *m_children[8];
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief  array of data points to be stored in tree, has x,y,z position and w Id of data's parent object
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Vec4> m_data;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief query if a box intersects with a sphere
    /// @param [in] Bmin the minimum corner of the box
    /// @param [in] Bmax the maximum corner of the box
    /// @param [in] C the centre of the sphere
    /// @param [in] r the radius of the circle
    /// @return true if box intersects with sphere, false if not
    //----------------------------------------------------------------------------------------------------------------------
    bool boxIntersectsSphere(ngl::Vec3 bMin, ngl::Vec3 bMax, ngl::Vec3 C, float r);
    int getDataSize();
    int m_dataIndex;
    int m_tempDataSize;
    int m_tempDataIndex;
    int getTempDataSize();
    int getTempDataIndex();
    int m_inc;



};

#endif // OCTREE_H
