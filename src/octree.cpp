#include <octree.h>
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <vector>
#include <stdlib.h>
#include <math.h>


Octree::Octree(ngl::Vec3 _origin, ngl::Vec3 _halfD, int _height)
{
    // set the position and size
    m_origin = _origin;
    m_halfD = _halfD;
    m_height = _height;
    //initially there are no children
    for(int i=0;i<8;++i)
            m_children[i] = NULL;
}

Octree::~Octree()
{
    // recursively delete all children
    for(int i=0;i<8;++i)
        delete m_children[i];
}

void Octree::setData(ngl::Vec4 _data)
{
    // set the data
    m_data.push_back(_data);
}

int Octree::getOctantContainingPoint(ngl::Vec3 point)
{
    // int identiying octant
    int oct = 0;
    // simple size comparison to determine the octant
    if(point.m_x >= m_origin.m_x)
        oct |= 4;
    if(point.m_y >= m_origin.m_y)
        oct |= 2;
    if(point.m_z >= m_origin.m_z)
        oct |= 1;
    return oct;
}

bool Octree::isLeaf()
{
    // The node is a leaf it it has no children. Since a node
    // has either none or all eight children we can just check
    // the first
    return (m_children[0] == NULL || m_height==1);
}

void Octree::insert(ngl::Vec4 _data)
{
    // If the node doesnt have a data point assigned and its a
    // leaf, assign the data
    if(isLeaf())
    {
        if(m_data.size()==0 || m_height==1)
        {
            m_data.push_back(_data);
            //std::cout<<"inserted :"<<m_data.back().m_w<<" at "<<m_height<<std::endl;
            return;
        }
        // If the leaf already has some data assigned; split the
        // node into 8 children, insert the old data along with
        // the new data point
        else
        {
            // Store old data point
            ngl::Vec4 oldData = m_data[0];
            m_data[0] = NULL;

            // Split the current node
            for(int i=0;i<8;++i)
            {
                // Compute new bounding box for this child
                ngl::Vec3 newOrigin = m_origin;
                newOrigin.m_x += m_halfD.m_x*(i&4 ? 0.5f : -0.5f);
                newOrigin.m_y += m_halfD.m_y*(i&2 ? 0.5f : -0.5f);
                newOrigin.m_z += m_halfD.m_z*(i&1 ? 0.5f : -0.5f);
                m_children[i] = new Octree(newOrigin, m_halfD*0.5f, (m_height -1));
            }

            // Re-insert old data and insert new data
            m_children[getOctantContainingPoint(oldData.toVec3())]->insert(oldData);
            m_children[getOctantContainingPoint(_data.toVec3())]->insert(_data);
        }
    }

    // If we are in an interior node; insert recursively into the
    // appropriate child octant
    else
    {
        int octant = getOctantContainingPoint(_data.toVec3());
        m_children[octant]->insert(_data);
    }
    return;
}

void Octree::getPointsInsideSphere(ngl::Vec3 centre, float radius, std::vector<ngl::Vec4> results)
{
    // If the current node is a leaf, check if the current data point
    // is inside the query bouding sphere
    if(isLeaf())
    {
        for(int i=0;i<m_data.size();++i)
        {
            if(sqrt(((m_data[i].m_x-centre.m_x)*(m_data[i].m_x-centre.m_x)+(m_data[i].m_y-centre.m_y)*(m_data[i].m_y-centre.m_y)+(m_data[i].m_z-centre.m_z)*(m_data[i].m_z-centre.m_z)))<=radius)
            {
                results.push_back(m_data[i]);
                std::cout<<"added neighbour"<<std::endl;
                //std::cout<<m_data.m_w<<std::endl;
                //std::cout<<"added data"<<std::endl;
            }
        }
        //std::cout<<m_results.size()<<std::endl;
    }
    // If we are in and interior node of the tree; check if the query
    // bouding sphere lies outside the octants of this node
    else
        for(int i=0;i<8;++i)
        {
            // Compute the min and max corners of this child octant
            ngl::Vec3 max = m_children[i]->m_origin + m_children[i]->m_halfD;
            ngl::Vec3 min = m_children[i]->m_origin - m_children[i]->m_halfD;

            // If the bounding sphere is inside of the child octant
            // then check the child
            if(boxIntersectsSphere(min, max, centre, radius))
                m_children[i]->getPointsInsideSphere(centre, radius, results);
        }
}

bool Octree::boxIntersectsSphere(ngl::Vec3 bMin, ngl::Vec3 bMax, ngl::Vec3 C, float r)
{
    float dist_squared = r * r;
    if (C.m_x < bMin.m_x)
        dist_squared -= (C.m_x - bMin.m_x)*(C.m_x - bMin.m_x);
    else if (C.m_x > bMax.m_x)
        dist_squared -= (C.m_x - bMax.m_x)*(C.m_x - bMax.m_x);
    if (C.m_y < bMin.m_y)
        dist_squared -= (C.m_y - bMin.m_y)*(C.m_y - bMin.m_y);
    else if (C.m_y > bMax.m_y)
        dist_squared -= (C.m_y - bMax.m_y)*(C.m_y - bMax.m_y);
    if (C.m_z < bMin.m_z)
        dist_squared -= (C.m_z - bMin.m_z)*(C.m_z - bMin.m_z);
    else if (C.m_z > bMax.m_z)
        dist_squared -= (C.m_z - bMax.m_z)*(C.m_z - bMax.m_z);
    return dist_squared > 0;
}


void Octree::clearResults()
{
    if(isLeaf())
    {
        m_results.clear();
        return;
    }
    else
    {
        for(int i=0;i<8;++i)
        {
            m_children[i]->clearResults();
        }
    }
}

//used for debugging
void Octree::findData(ngl::Vec3 data)
{
    if(isLeaf())
    {
        if(m_data[0]!=NULL)
        {
            std::cout<<"found data here: "<<std::endl;
        }
    }
    else
    {
        for(int i=0;i<8;++i)
        {
            std::cout<<"searching child octant "<<i<<std::endl;
            m_children[i]->findData(data);
        }
    }
}
