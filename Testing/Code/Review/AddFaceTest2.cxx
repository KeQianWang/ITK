#include "itkQEMesh.h"
#include "itkQEUtils.txx"

int AddFaceTest2(int , char *[])
{
  typedef itkQE::Mesh< double, 3 >        MeshType;
  typedef MeshType::Pointer               MeshPointer;
  typedef MeshType::QEType                QEType;
  typedef MeshType::PointIdentifier       PointIdentifier;
  typedef MeshType::CellType              CellType;
  typedef itkQE::PolygonCell< CellType >  QEPolygonCellType;

  int numPts = 7;
  int numCells = 4;

  std::cout << "numPts= " << numPts << std::endl;
  std::cout << "numCells= " << numCells << std::endl;

  int oddConnectivityCells[12] =
  { 0, 1, 2,
    3, 4, 5,
    6, 4, 0,
    0, 4, 6, };

  // Configuration of odd_connectivity mesh:
  // numVertices=7 , numEdges=9, numFaces=3, numBoundary=1, Chi=1
  //
  //    5 ---------- 4 ---------- 0 ---------- 1
  //    |  [2]   __/ |   [3]  __/   \__  [1]   |
  //    |     __/    |     __/         \__     |
  //    |  __/       |  __/               \__  |
  //    | /          | /                     \ |
  //    3            6                         2
  //
  // First we add the three triangles [0 1 2], [3 4 5], [6 4 0].
  // Then we try to add a fourth triangle, which is the third triangle
  // inverted i.e. [0 4 6]. This triangle can not be added.

  MeshPointer  mesh = MeshType::New();

  MeshType::PointType pts[7];

  pts[0][0] = 2.0;  pts[0][1] = 1.0;  pts[0][2] = 0.0;
  pts[1][0] = 3.0;  pts[1][1] = 1.0;  pts[1][2] = 0.0;
  pts[2][0] = 3.0;  pts[2][1] = 0.0;  pts[2][2] = 0.0;
  pts[3][0] = 0.0;  pts[3][1] = 0.0;  pts[3][2] = 0.0;
  pts[4][0] = 1.0;  pts[4][1] = 1.0;  pts[4][2] = 0.0;
  pts[5][0] = 0.0;  pts[5][1] = 1.0;  pts[5][2] = 0.0;
  pts[6][0] = 1.0;  pts[6][1] = 0.0;  pts[6][2] = 0.0;

  for(int i=0; i<numPts; i++)
    {
    mesh->SetPoint( i, pts[i] );
    }

  int computedNumPts = mesh->GetNumberOfPoints();
  std::cout << "computedNumPts= " << computedNumPts << std::endl;

  CellType::CellAutoPointer cellpointer;
  QEPolygonCellType *poly;

  for(int i=0; i<numCells; i++)
    {
    poly = new QEPolygonCellType(3);
    cellpointer.TakeOwnership( poly );
    cellpointer->SetPointId( 0, oddConnectivityCells[3*i] );
    cellpointer->SetPointId( 1, oddConnectivityCells[3*i+1] );
    cellpointer->SetPointId( 2, oddConnectivityCells[3*i+2] );
    mesh->SetCell( i, cellpointer );
    }

  int computedNumCells = mesh->GetNumberOfCells();
  std::cout << "computedNumCells= " << computedNumCells << std::endl;

  std::cout << "Test whether the fourth face was rejected" << std::endl;

  if( itkQE::AssertTopologicalInvariants< MeshType >
      ( mesh, 7, 9, 3, 1, 1 ) )
    {
    std::cout << "Passed" << std::endl;
    }
  else
    {
    std::cout << "Failed" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
