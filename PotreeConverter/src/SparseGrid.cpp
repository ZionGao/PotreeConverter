
#include "SparseGrid.h"


SparseGrid::SparseGrid(AABB aabb, float minGap){
	this->aabb = aabb;
	this->width = aabb.size.x / minGap;
	this->height = aabb.size.y / minGap;
	this->depth = aabb.size.z / minGap;
	this->minGap = minGap;
}

SparseGrid::~SparseGrid(){
	SparseGrid::iterator it;
	for(it = begin(); it != end(); it++){
		delete it->second;
	}
}


/**
 * returns target and neighbour cells
 */
vector<GridCell*> SparseGrid::targetArea(int x, int y, int z){
	vector<GridCell*> cells;

	for(int i = x - 1; i <= x + 1; i++){
		for(int j = y - 1; j <= y + 1; j++){
			for(int k = z - 1; k <= z + 1; k++){
				GridIndex index(i,j,k);
				if(find(index) != end()){
					cells.push_back(this->operator[](index));
				}
			}
		}
	}

	return cells;
}

bool SparseGrid::isDistant(const Vector3<double> &p, int i, int j, int k){
	GridCell *cell = this->operator[](GridIndex(i,j,k));
	if(cell->minGap(p) < minGap){
		return false;
	}
	for(int a = 0; a < cell->neighbours.size(); a++){
		GridCell *neighbour = cell->neighbours[a];
		float gap = neighbour->minGap(p);
		if(gap < minGap){
			return false;
		}
	}

	return true;
}

bool SparseGrid::add(Vector3<double> &p){
	int nx = (int)(width*(p.x - aabb.min.x) / aabb.size.x);
	int ny = (int)(height*(p.y - aabb.min.y) / aabb.size.y);
	int nz = (int)(depth*(p.z - aabb.min.z) / aabb.size.z);

	int i = min(nx, width-1);
	int j = min(ny, height-1);
	int k = min(nz, depth-1);

	GridIndex index(i,j,k);
	if(find(index) == end()){
		this->operator[](index) = new GridCell(this, index);
	}

	if(isDistant(p, i, j, k)){
		this->operator[](index)->add(p);
		return true;
	}else{
		return false;
	}
}

void SparseGrid::addWithoutCheck(Vector3<double> &p){
	int nx = (int)(width*(p.x - aabb.min.x) / aabb.size.x);
	int ny = (int)(height*(p.y - aabb.min.y) / aabb.size.y);
	int nz = (int)(depth*(p.z - aabb.min.z) / aabb.size.z);

	int i = min(nx, width-1);
	int j = min(ny, height-1);
	int k = min(nz, depth-1);

	GridIndex index(i,j,k);
	if(find(index) == end()){
		this->operator[](index) = new GridCell(this, index);
	}

	this->operator[](index)->add(p);
}