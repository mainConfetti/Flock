#include <octree.h>
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <vector>
#include <stdlib.h>
#include <boidmath.h>
#include <algorithm>



Octree::Octree(ngl::Vec3 _origin, ngl::Vec3 _halfD, int _height)
{
    // set the position and size
    m_origin = _origin;
    m_halfD = _halfD;
    m_height = _height;
    //initially there are no children
    m_children.clear();
    m_dataIndex = 0;
    m_tempDataSize = 0;
    m_tempDataIndex = 0;
    m_inc=0;
}

Octree::~Octree()
{
    // recursively delete all children
    m_children.clear();
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
    // The node is a leaf it it has no children or its
    // height is 1
    return (m_height==1 || m_children.size()==0);
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

        }
        // If the leaf already has some data assigned; split the
        // node into 8 children, insert the old data along with
        // the new data point
        else
        {
            // each time we split a node, we will have to itterate over it
            // one more time to retrieve all data in its chilren when calling
            // getObjectsInSphere.
            ++m_tempDataIndex;
            // Store old data point
            ngl::Vec4 oldData = m_data[0];
            m_data.clear();

            // Split the current node
            for(int i=0;i<8;++i)
            {
                // Compute new bounding box for this child
                ngl::Vec3 newOrigin = m_origin;
                newOrigin.m_x += m_halfD.m_x*(i&4 ? 0.5f : -0.5f);
                newOrigin.m_y += m_halfD.m_y*(i&2 ? 0.5f : -0.5f);
                newOrigin.m_z += m_halfD.m_z*(i&1 ? 0.5f : -0.5f);
                Octree child (newOrigin, m_halfD*0.5f, (m_height -1));
                m_children.push_back(child);
            }

            // Re-insert old data and insert new data
            //std::cout<<"entering: "<<m_height-1<<" : "<<getOctantContainingPoint(oldData.toVec3())<<std::endl;
            m_children[getOctantContainingPoint(oldData.toVec3())].insert(oldData);
            //std::cout<<"entering: "<<m_height-1<<" : "<<getOctantContainingPoint(_data.toVec3())<<std::endl;
            m_children[getOctantContainingPoint(_data.toVec3())].insert(_data);
        }
    }

    // If we are in an interior node; insert recursively into the
    // appropriate child octant
    else
    {
        int octant = getOctantContainingPoint(_data.toVec3());
        // each time we split a node, we will have to itterate over it
        // one more time to retrieve all data in its chilren when calling
        // getObjectsInSphere.
        ++m_tempDataIndex;
        //std::cout<<"entering: "<<m_height-1<<" : "<<octant<<std::endl;
        m_children[octant].insert(_data);
    }
    return;
}

ngl::Vec4 Octree::getPointsInsideSphere(ngl::Vec3 centre, float radius)
{
    // If the current node is a leaf, check if the current data point
    // is inside the query bouding sphere
    int inc;
    ngl::Vec4 tempResults=NULL;
    ngl::Vec4 temp;
    if(isLeaf())
    {
        //std::cout<<"size before: "<<temp.size()<<std::endl;
        if(sqrt(((m_data[m_dataIndex].m_x-centre.m_x)*(m_data[m_dataIndex].m_x-centre.m_x)+(m_data[m_dataIndex].m_y-centre.m_y)*(m_data[m_dataIndex].m_y-centre.m_y)+(m_data[m_dataIndex].m_z-centre.m_z)*(m_data[m_dataIndex].m_z-centre.m_z)))<=radius)
        {
            tempResults=m_data[m_dataIndex];
            //std::cout<<m_data[m_dataIndex].m_w<<" added neighbour: "<<tempResults->m_w<<std::endl;
            //std::cout<<m_data.m_w<<std::endl;
            //std::cout<<"added data"<<std::endl;
        }
        ++m_dataIndex;
    }
    // If we are in and interior node of the tree; check if the query
    // bouding sphere lies outside the octants of this node
    else
    {
        if(m_inc==0)
            for(int i=0;i<8;++i)
            {
                // Compute the min and max corners of this child octant
                ngl::Vec3 max = m_children[i].m_origin + m_children[i].m_halfD;
                ngl::Vec3 min = m_children[i].m_origin - m_children[i].m_halfD;
                // If the bounding sphere is inside of the child octant
                // then check the child
                if(BoidMath::boxIntersectsSphere(min, max, centre, radius))
                {
                    inc = m_children[i].getTempDataIndex()+1;
                    if(m_children[i].isLeaf())
                        inc = m_children[i].getDataSize();
                    for(int j=0;j<inc;++j)
                    {
                        temp =m_children[i].getPointsInsideSphere(centre, radius);
                        if(temp!=NULL)
                        {
                            temp_data.push_back(temp);
                        }
                    }
                 }
            }
        //int index = m_plusIndex-m_tempDataIndex;
        //index+=m_inc;
        if(temp_data.size()!=0 && temp_data.size()>m_inc)
            tempResults=temp_data[m_inc];
        ++m_inc;
        ++m_tempDataSize;
    }
    return tempResults;
}

void Octree::clearResults()
{
    if(isLeaf())
    {
        temp_data.clear();
        m_dataIndex = 0;
        m_tempDataSize=0;
        m_inc = 0;
        return;
    }
    else
    {
        temp_data.clear();
        m_dataIndex = 0;
        m_tempDataSize=0;
        m_inc = 0;
        for(int i=0;i<8;++i)
        {
            m_children[i].clearResults();
        }
    }
    return;
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
            m_children[i].findData(data);
        }
    }
}

int Octree::getDataSize()
{
    return m_data.size();
}

int Octree::getTempDataSize()
{
    return m_tempDataSize;
}

int Octree::getTempDataIndex()
{
    return m_tempDataIndex;
}

void Octree::cleanResults()
{
    //temp_data.erase(std::remove(temp_data.begin(), temp_data.end(), NULL), temp_data.end());
    return;
}
