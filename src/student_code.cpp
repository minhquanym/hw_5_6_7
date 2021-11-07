#include "student_code.h"
#include "mutablePriorityQueue.h"

using namespace std;

namespace CGL
{

  /**
   * Evaluates one step of the de Casteljau's algorithm using the given points and
   * the scalar parameter t (class member).
   *
   * @param points A vector of points in 2D
   * @return A vector containing intermediate points or the final interpolated vector
   */
  Vector2D lerp(Vector2D u, Vector2D v, double t) {
    return u + (v - u) * t;
  }

  std::vector<Vector2D> BezierCurve::evaluateStep(std::vector<Vector2D> const &points)
  { 
    // TODO Part 1.
    std::vector<Vector2D> result;
    for (int i = 0; i + 1 < points.size(); i++) {
      result.push_back(lerp(points[i], points[i + 1], t));
    }
    return result;
  }

  Vector3D lerp(Vector3D u, Vector3D v, double t) {
    return u + (v - u) * t;
  }

  /**
   * Evaluates one step of the de Casteljau's algorithm using the given points and
   * the scalar parameter t (function parameter).
   *
   * @param points    A vector of points in 3D
   * @param t         Scalar interpolation parameter
   * @return A vector containing intermediate points or the final interpolated vector
   */
  std::vector<Vector3D> BezierPatch::evaluateStep(std::vector<Vector3D> const &points, double t) const
  {
    // TODO Part 2.
    std::vector<Vector3D> result;
    for (int i = 0; i + 1 < points.size(); i++) {
      result.push_back(lerp(points[i], points[i + 1], t));
    }
    return result;
  }

  /**
   * Fully evaluates de Casteljau's algorithm for a vector of points at scalar parameter t
   *
   * @param points    A vector of points in 3D
   * @param t         Scalar interpolation parameter
   * @return Final interpolated vector
   */
  Vector3D BezierPatch::evaluate1D(std::vector<Vector3D> const &points, double t) const
  {
    // TODO Part 2.
    std::vector<Vector3D> result = points;
    while (result.size() > 1) {
      result = evaluateStep(result, t);
    }
    return result[0];
  }

  /**
   * Evaluates the Bezier patch at parameter (u, v)
   *
   * @param u         Scalar interpolation parameter
   * @param v         Scalar interpolation parameter (along the other axis)
   * @return Final interpolated vector
   */
  Vector3D BezierPatch::evaluate(double u, double v) const 
  {  
    // TODO Part 2.
    std::vector<Vector3D> rowPoints;
    for (int i = 0; i < controlPoints.size(); i++) {
      rowPoints.push_back(evaluate1D(controlPoints[i], u));
    }
    return evaluate1D(rowPoints, v);
  }

  Vector3D Vertex::normal( void ) const
  {
    // TODO Part 3.
    // Returns an approximate unit normal at this vertex, computed by
    // taking the area-weighted average of the normals of neighboring
    // triangles, then normalizing.
    Vector3D res = Vector3D(0, 0, 0);
    HalfedgeCIter h = this->halfedge();
    do {
      Vector3D u = h->next()->vertex()->position - h->vertex()->position;
      Vector3D v = h->next()->next()->vertex()->position - h->vertex()->position;
      res += cross(u, v);
      h = h->twin()->next();
    } while (h != this->halfedge());
    return res.unit();
  }

  EdgeIter HalfedgeMesh::flipEdge( EdgeIter e0 )
  {
    if (e0->isBoundary())
  	  return e0;
  
    HalfedgeIter h0 = e0->halfedge();
    HalfedgeIter h1 = h0->next();
    HalfedgeIter h2 = h1->next();

    HalfedgeIter h3 = h0->twin();
    HalfedgeIter h4 = h3->next();
    HalfedgeIter h5 = h4->next();

    HalfedgeIter h6 = h5->twin();
    HalfedgeIter h7 = h4->twin();
    HalfedgeIter h8 = h2->twin();
    HalfedgeIter h9 = h1->twin();
  
    VertexIter v0 = h0 -> vertex();
    VertexIter v1 = h3 -> vertex();
    VertexIter v2 = h5 -> vertex();
    VertexIter v3 = h2 -> vertex();
  
    EdgeIter e1 = h5 -> edge();
    EdgeIter e2 = h4 -> edge();
    EdgeIter e3 = h2 -> edge();
    EdgeIter e4 = h1 -> edge();
  
    FaceIter f0 = h0 -> face();
    FaceIter f1 = h3 -> face();
  
    v0 -> halfedge() = h4;
    v1 -> halfedge() = h1;
    v2 -> halfedge() = h5;
    v3 -> halfedge() = h2;
  
    e0 -> halfedge() = h0;
    e1 -> halfedge() = h5;
    e2 -> halfedge() = h4;
    e3 -> halfedge() = h2;
    e4 -> halfedge() = h1;
  
    f0 -> halfedge() = h0;
    f1 -> halfedge() = h3;
  
  
    h0 -> setNeighbors(h2, h3, v2, e0, f0);
    h1 -> setNeighbors(h3, h9, v1, e4, f1);
    h2 -> setNeighbors(h4, h8, v3, e3, f0);
    h3 -> setNeighbors(h5, h0, v3, e0, f1);
    h4 -> setNeighbors(h0, h7, v0, e2, f0);
    h5 -> setNeighbors(h1, h6, v2, e1, f1);
    
    h6 -> setNeighbors(h6 -> next(), h5, v1, e1, h6 -> face());
    h7 -> setNeighbors(h7 -> next(), h4, v2, e2, h7 -> face());
    h8 -> setNeighbors(h8 -> next(), h2, v0, e3, h8 -> face());
    h9 -> setNeighbors(h9 -> next(), h1, v3, e4, h9 -> face());

    return e0;
  }

  VertexIter HalfedgeMesh::splitEdge( EdgeIter e0 )
  {
    if (e0->isBoundary())
      return e0->halfedge()->vertex();
      
    HalfedgeIter h0 = e0->halfedge();
    HalfedgeIter h1 = h0->next();
    HalfedgeIter h2 = h1->next();
    HalfedgeIter h3 = h0->twin();
    HalfedgeIter h4 = h3->next();
    HalfedgeIter h5 = h4->next();

    HalfedgeIter h6 = h1->twin();
    HalfedgeIter h7 = h2->twin();
    HalfedgeIter h8 = h4->twin();
    HalfedgeIter h9 = h5->twin();
  
    VertexIter v0 = h0 -> vertex();
    VertexIter v1 = h3 -> vertex();
    VertexIter v2 = h2 -> vertex();
    VertexIter v3 = h5 -> vertex();
  
    EdgeIter e1 = h1->edge();
    EdgeIter e2 = h2->edge();
    EdgeIter e3 = h4->edge();
    EdgeIter e4 = h5->edge();
  
    FaceIter f0 = h0->face();
    FaceIter f1 = h3->face();
  
    EdgeIter e5 = newEdge();
    EdgeIter e6 = newEdge();
    EdgeIter e7 = newEdge();
  
    FaceIter f2 = newFace();
    FaceIter f3 = newFace();
  
    HalfedgeIter h10 = newHalfedge();
    HalfedgeIter h11 = newHalfedge();
    HalfedgeIter h12 = newHalfedge();
    HalfedgeIter h13 = newHalfedge();
    HalfedgeIter h14 = newHalfedge();
    HalfedgeIter h15 = newHalfedge();

    VertexIter v4 = newVertex();
    v4 -> position = (v0 -> position + v1 -> position) * 0.5;
    v4 -> isNew = 1;

    f0 -> halfedge() = h0;
    f1 -> halfedge() = h3;
    
    h0->setNeighbors(h1,h3,v4,e0,f0);
    h1->setNeighbors(h2,h6,v1,e1,f0);
    h2->setNeighbors(h0,h11,v2,e5,f0);
    h3->setNeighbors(h4,h0,v1,e0,f1);
    h4->setNeighbors(h5,h15,v4,e7,f1);
    h5->setNeighbors(h3,h9,v3,e4,f1);
    h6->setNeighbors(h6->next(),h1,v2,e1,h6->face());
    h7->setNeighbors(h7->next(),h12,v0,e2,h7->face());
    h8->setNeighbors(h8->next(),h14,v3,e3,h8->face());
    h9->setNeighbors(h9->next(),h5,v1,e4,h9->face());
    h10->setNeighbors(h11,h13,v0,e6,f2);
    h11->setNeighbors(h12,h2,v4,e5,f2);
    h12->setNeighbors(h10,h7,v2,e2,f2);
    h13->setNeighbors(h14,h10,v4,e6,f3);
    h14->setNeighbors(h15,h8,v0,e3,f3);
    h15->setNeighbors(h13,h4,v3,e7,f3);

    v0->halfedge() = h10;
    v1->halfedge() = h1;
    v2->halfedge() = h12;
    v3->halfedge() = h5;
    v4->halfedge() = h0;
    
    e0->halfedge() = h0;
    e1->halfedge() = h1;
    e2->halfedge() = h12;
    e3->halfedge() = h14;
    e4->halfedge() = h5;
    e5->halfedge() = h2;
    e6->halfedge() = h10;
    e7->halfedge() = h4;

    e0-> isNew = 0;
    e6-> isNew = 0;
    e5-> isNew = 1;
    e7-> isNew = 1;

    f0->halfedge() = h0;
    f1->halfedge() = h3;
    f2->halfedge() = h10;
    f3->halfedge() = h13;

    return v4;
  }



  void MeshResampler::upsample( HalfedgeMesh& mesh )
  {
    // TODO Part 6.
    // This routine should increase the number of triangles in the mesh using Loop subdivision.
    // One possible solution is to break up the method as listed below.

    // 1. Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule,
    // and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being
    // a vertex of the original mesh.
    
    // 2. Compute the updated vertex positions associated with edges, and store it in Edge::newPosition.
    
    // 3. Split every edge in the mesh, in any order. For future reference, we're also going to store some
    // information about which subdivide edges come from splitting an edge in the original mesh, and which edges
    // are new, by setting the flat Edge::isNew. Note that in this loop, we only want to iterate over edges of
    // the original mesh---otherwise, we'll end up splitting edges that we just split (and the loop will never end!)
    
    // 4. Flip any new edge that connects an old and new vertex.

    // 5. Copy the new vertex positions into final Vertex::position.
    HalfedgeIter h;
    VertexIter a, b, c, d;
    EdgeIter e1, e2, e3;
    FaceIter f1, f2, f3;
    int edgeNum = 0;

    // 2. Compute the updated vertex positions associated with edges, and store it in Edge::newPosition.
    for (auto e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
      e->isNew = false;
      edgeNum++;
      h = e->halfedge();
      a = h->vertex();
      c = h->twin()->vertex();
      b = h->next()->next()->vertex();
      d = h->twin()->next()->next()->vertex();
      e->newPosition = ((a->position + c->position) * 3 + (b->position + d->position)) / 8;
    }

    // 1. Compute new positions for all the vertices in the input mesh
    for (auto v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
      v->isNew = false;
      Vector3D sum = Vector3D(0, 0, 0);
      h = v->halfedge();
      do {
        sum += h->twin()->vertex()->position;
        h = h->twin()->next();
      } while (h != v->halfedge());

      int n = v->degree();
      double u;
      if (n == 3) {
        u = 3.0 / 16.0;
      } else {
        u = 3.0 / (8.0 * n);
      }

      v->newPosition = (1.0 - n * u) * v->position + u * sum;
    }

    // Split edge
    for (auto e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
      VertexIter v1 = e->halfedge()->vertex();
      VertexIter v2 = e->halfedge()->twin()->vertex();

      // Split edge if only its an old edge
      if (!(v1->isNew || v2->isNew)) {
        auto v = mesh.splitEdge(e);
        v->newPosition = e->newPosition;
      }
    }

    // Flip new edge that connects old and new vertex
    for (auto e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
      if (e->isNew) {
        h = e->halfedge();
        if ((h->vertex()->isNew && !h->twin()->vertex()->isNew) || (!h->vertex()->isNew && h->twin()->vertex()->isNew)) {
          mesh.flipEdge(e);
        }
      }
    }

    // Assign new position to vertex
    for (auto v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
      v->position = v->newPosition;
    }
  }
}
