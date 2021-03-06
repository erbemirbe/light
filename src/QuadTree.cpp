#include "LTBL/QuadTree.h"

#include "LTBL/SFML_OpenGL.h"

using namespace qdt;

QuadTree::QuadTree(const AABB &startRegion)
{
  rootNode = new QuadTreeNode(startRegion, 1, NULL, this);
}

QuadTree::~QuadTree()
{
  delete rootNode;
}

void QuadTree::addOccupant(QuadTreeOccupant* pOc)
{
  if(rootNode->region.contains(pOc->aabb)) // If it fits inside the root node
    rootNode->addOccupant(pOc);
  else // Otherwise, add it to the outside root set
  {
    outsideRoot.insert(pOc);

    // Set the pointers properly
    pOc->pQuadTreeNode = NULL; // Not required unless removing a node and then adding it again
    pOc->pQuadTree = this;
  }
}

void QuadTree::clearTree(const AABB &newStartRegion)
{
  delete rootNode;
  rootNode = new QuadTreeNode(newStartRegion, 1, NULL, this);

  // Clear ouside root
  outsideRoot.clear();
}

void QuadTree::query(const AABB &queryRegion, std::vector<QuadTreeOccupant*> &queryResult)
{
  // First parse the occupants outside of the root and
  // add them to the array if the fit in the query region
  for(std::unordered_set<QuadTreeOccupant*>::iterator it = outsideRoot.begin(); it != outsideRoot.end(); it++)
    if((*it)->aabb.intersects(queryRegion))
      queryResult.push_back(*it);

  // Then query the tree itself
  rootNode->query(queryRegion, queryResult);
}

void QuadTree::queryToDepth(const AABB &queryRegion, std::vector<QuadTreeOccupant*> &queryResult, int depth)
{
  // First parse the occupants outside of the root and
  // add them to the array if the fit in the query region
  for(std::unordered_set<QuadTreeOccupant*>::iterator it = outsideRoot.begin(); it != outsideRoot.end(); it++)
    if((*it)->aabb.intersects(queryRegion))
      queryResult.push_back(*it);

  // Then query the tree itself
  rootNode->queryToDepth(queryRegion, queryResult, depth);
}

unsigned int QuadTree::getNumOccupants()
{
  return rootNode->numOccupants;
}

AABB QuadTree::getRootAABB()
{
  return rootNode->region;
}

void QuadTree::debugRender()
{
  glColor4f(0.1f, 0.6f, 0.4f, 1.0f);

  // Parse all AABB's in the tree and render them
  for(std::unordered_set<QuadTreeOccupant*>::iterator it = outsideRoot.begin(); it != outsideRoot.end(); it++)
    (*it)->aabb.debugRender();

  // Render the tree itself
  rootNode->debugRender();

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
