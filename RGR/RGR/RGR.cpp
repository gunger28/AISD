#include "stdafx.h"
#include "locale.h"
#include "iostream"
#include "conio.h"
#include "stdlib.h"
#include "string"
#include "windows.h"
#include "vector"
#include "time.h"
#include "map"
#include "list"
#include "iomanip"
#include "limits"
#include "functional"
#include "queue"
#include <sstream>
#include <iostream>

using namespace std;

int sz=0;

string i_to_s(int x){
    char buffer[20];
    _itoa(x, buffer, 10);         // 10 - это система счисления
    string s = buffer; 
    return s;
}

int minimum(int x1, int x2){
	if (x1<x2)
		return x1;
	return x2;
};

//Вершина

template <class NameT, class DataT>
class Vertex {
    NameT name;     //Имя вершины
    DataT data;     //Данные вершины
public:
	 bool nameSet, dataSet;
    
	//Конструкторы
    Vertex() : nameSet(false), dataSet(false) {}
    Vertex(NameT name, DataT data) : name(name), data(data), nameSet(true), dataSet(true) {}
    
	//Установка, получение имени и данных
    void setName(NameT name) { 
		this->name = name; 
		nameSet = true; 
	}
    void setData(DataT data) { 
		this->data = data; 
		dataSet = true; 
	}
    NameT getName() { 
		return name; 
	}
    DataT getData() { 
		return data; 
	}
    bool isNameSet() { 
		return nameSet; 
	}
    bool isDataSet() { 
		return dataSet; 
	}
};
//====================================

template <class VertexT, class WeightT, class DataT>
class Edge {
    VertexT *v1, *v2;   //Вершины, которые соединяет ребро
    WeightT weight;     //Вес
    DataT data;         //Данные ребра 
	bool weightSet, dataSet;
public:
    //Конструкторы
    Edge(VertexT *v1, VertexT *v2) : v1(v1), v2(v2), weightSet(false), weight(0), dataSet(false) {}
    Edge(VertexT *v1, VertexT *v2, WeightT weight) : v1(v1), v2(v2), weight(weight), weightSet(true), dataSet(false) {}
    Edge(VertexT *v1, VertexT *v2, WeightT weight, DataT data) : v1(v1), v2(v2), weight(weight), data(data), weightSet(true), dataSet(true) {}
    
	//Установка и получение веса и данных
    void setWeight(WeightT weight) { 
		this->weight = weight; 
		weightSet = true; 
	}
    void setData(DataT data) { 
		this->data = data; 
		dataSet = true; 
	}
    WeightT getWeight() { 
		return weight; 
	}
    DataT getData() { 
		return data; 
	}
	VertexT* getVertex1(){
		return v1;
	};
	VertexT* getVertex2(){
		return v2;
	};
    bool isWeightSet() { 
		return weightSet; 
	}
    bool isDataSet() { 
		return dataSet; 
	}
};

//====================
// Представление графа
template <class EdgeT>
class GraphForm {
public:
    //Вставка и удаление вершин и рёбер
    virtual bool insertVertex(int index) = 0;
    virtual bool deleteVertex(int index) = 0;
    virtual bool insertEdge(int v1, int v2, EdgeT *t) = 0;
    virtual bool deleteEdge(int v1, int v2) = 0;
    //Удалить входящие и исходящие из вершины рёбра
    virtual int deleteEdgesFromVertex(int index, bool directed) = 0;
    //Проверка и получение
    virtual bool hasEdge(int v1, int v2) = 0;
    virtual EdgeT* getEdge(int v1, int v2) = 0;
};

//Матричная форма

template <class EdgeT>
class GraphMatrixForm : public GraphForm<EdgeT> {
    friend class EdgeIterator;
    bool directed;
    vector<vector<EdgeT*>> matrix; //Матрица смежности
public:
    GraphMatrixForm(bool directed) : directed(directed) {}
    //Вставка и удаление вершин и рёбер
  
	bool insertVertex(int index) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if(index < 0 || index > size) return false;
        //Создаём новую пустую строку
        vector<EdgeT*> newLine;
        newLine.assign(size, NULL);
        //Вставляем новую строку:
        matrix.insert(matrix.begin() + index, newLine);
        ++size;
        //Вставляем новый столбец:
        for(int i = 0; i < size; ++i)
            matrix[i].insert(matrix[i].begin() + index, (EdgeT*)NULL);
        return true;
    }
    bool deleteVertex(int index) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if(index < 0 || index >= size) return false;
        //Удаляем строку:
        matrix.erase(matrix.begin() + index);
        --size;
        //Удаляем столбец:
        for(int i = 0; i < size; i++)
            matrix[i].erase(matrix[i].begin() + index);
        return true;
    }
    bool insertEdge(int v1, int v2, EdgeT *t) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) return false;
        //Петля или ребро уже есть
        if(v1 == v2 || matrix[v1][v2] != NULL) return false;
        //Вставляем ребро
        matrix[v1][v2] = t;
        return true;
    }
    bool deleteEdge(int v1, int v2) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) return false;
        //Ребра нет
        if(v1 == v2 || matrix[v1][v2] == NULL) return false;
        matrix[v1][v2] = NULL;
        return true;
    }
    //Удалить входящие и исходящие из вершины рёбра
    int deleteEdgesFromVertex(int index, bool directed) {
        int size = matrix.size(); //Число вершин
        int deleted = 0;
        //Неверный номер вершины
        if(index < 0 || index >= size) return 0;
        //Удаляем связанные с вершиной рёбра
        for(int i = 0; i < size; i++) {
            if(matrix[i][index] != NULL) {
                delete matrix[i][index];
                matrix[i][index] = NULL;
                ++deleted;
                //Стираем симметричное ребро
                if(directed == false)
                    matrix[index][i] = NULL;
            }
            if(matrix[index][i] != NULL) {
                delete matrix[index][i];
                matrix[index][i] = NULL;
                ++deleted;
            }
        }
        return deleted;
    }
    //Проверка и получение
    bool hasEdge(int v1, int v2) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			return false;
        if(v1 == v2) //Петля
			return false;
        if(matrix[v1][v2] != NULL) 
			return true;
        return false;
    }
    EdgeT* getEdge(int v1, int v2) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			throw 1;
        if(v1 == v2 || matrix[v1][v2] == NULL)//Петля 
			throw 1;
        return matrix[v1][v2];
    }

    //Итератор рёбер

    class EdgeIterator {
        bool end;
        GraphMatrixForm *f;
    public:
		int curI, curJ;
        EdgeIterator(GraphMatrixForm *f) {
            this->f = f;
            begin();
        }
        bool begin() {
            for(int i = 0; i < f->matrix.size(); ++i)
                for(int j = 0; j < f->matrix.size(); ++j)
                    if(f->matrix[i][j]) {
                        curI = i;
                        curJ = j;
                        end = false;
                        return true;
                    }
					end = true;
					return false;
        }
        bool onEnd() { 
			return end; 
		}
		bool toend(){
			for(int i=f->matrix.size()-1; i>=0; --i)
				for(int j=f->matrix.size()-1; j>=0; --j)
					if (f->matrix[i][j]){
						curI=i;
						curJ=j;
						end=false;
						return true;
					}
					end=true;
					return false;
		}
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curI < f->matrix.size()) {
                while(curJ < f->matrix.size()) {
                    if(f->matrix[curI][curJ])
                        return true;
                    ++curJ;
                }
                ++curI;
                curJ = (f->directed ? 0 : curI + 1);
            }
            end = true;
            return false;
        }
        EdgeT* getEdge() {
            if(end)
                throw 1;
			if (!(f->matrix[curI][curJ]))
				throw 1;
            return f->matrix[curI][curJ];
        }
    };

    //Итератор исходящих рёбер
    
	class OutputEdgeIterator {
        int curI, curJ;
        bool end;
        GraphMatrixForm *f;
    public:
        OutputEdgeIterator(GraphMatrixForm *f, int vIndex) {
            this->f = f;
            curI = vIndex;
            begin();
        }
        bool begin() {
            for(curJ = 0; curJ < f->matrix.size(); ++curJ)
                if(f->matrix[curI][curJ]) {
                    end = false;
                    return true;
                }
            end = true;
            return false;
        }
		bool toend(){
			for(curJ=f->matrix.size()-1; curJ>=0; --curJ)
				if (f->matrix[curI][curJ]){
					end=false;
					return true;
				}
				end=true;
				return false;
		}
        bool onEnd() { 
			return end; 
		}
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curJ < f->matrix.size()) {
                if(f->matrix[curI][curJ])
                    return true;
                ++curJ;
            }
            end = true;
            return false;
        }
        EdgeT* getEdge() {
            if(end)
                throw 1;
            return f->matrix[curI][curJ];
        }
    };
};

//===========

//Списочная форма

template <class EdgeT>
class GraphListForm : public GraphForm<EdgeT> {
public:
	//Элемент списка
	class Node {
	public:
		EdgeT* edge; //Само ребро
		int v2;      //Вторая вершина, которую ребро соединяет
	};
	vector<list<Node>> edgeList; //Списки смежности
	bool directed;
	
	GraphListForm(bool directed) : directed(directed) {}
    //Вставка и удаление вершин и рёбер
    bool insertVertex(int index) {
        int size = edgeList.size(); //Число вершин
        if(index < 0 || index > size) //Неверный номер вершины
			return false;
        //Создаём новый список смежности
        list<Node> newList;
        //Вставляем
        edgeList.insert(edgeList.begin() + index, newList);
        ++size;
        //Обновляем дескрипторы
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 >= index)//если текущая вершина имеет больший но-мер, чем вставляемая,
                    ++((*j).v2);//то увеличиваем этот номер
        return true;
    }
    bool deleteVertex(int index) {
        int size = edgeList.size(); //Число вершин
        if(index < 0 || index >= size)  //Неверный номер вершины
			return false;
        //Удаляем из списков записи о рёбрах
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 == index) {
                    edgeList[i].erase(j);
                    break;
                }
        //Удаляем список смежности
        edgeList.erase(edgeList.begin() + index);
        --size;
        //Обновляем дескрипторы
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 > index)//если текущая вершина имеет больший номер, чем удаляемая,
                    --((*j).v2);//то уменьшить этот номер
        return true;
    }
    bool insertEdge(int v1, int v2, EdgeT *t) {
        int size = edgeList.size(); //Число вершин
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)//Неверный номер вер-шины
			return false;
        if(v1 == v2 || hasEdge(v1, v2))  //Петля или ребро уже есть
			return false;
        //Вставляем ребро
        Node node;
        node.edge = t;
        node.v2 = v2;
        edgeList[v1].push_front(node);
        return true;
    }
    bool deleteEdge(int v1, int v2) {
        int size = edgeList.size(); //Число вершин
        //Неверный номер вершины
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			return false;
        //Ребра нет
        if(v1 == v2 || hasEdge(v1, v2) == false) 
			return false;
        //Удаляем ребро
        for(list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if((*j).v2 == v2) {
                edgeList[v1].erase(j);
                break;
            }
        return true;
    }
    //Удалить входящие и исходящие из вершины рёбра
    int deleteEdgesFromVertex(int index, bool directed) {
        int size = edgeList.size(); //Число вершин
        int deleted = 0;
        //Неверный номер вершины
        if(index < 0 || index >= size) 
			return 0;
        //Удаляем связанные с вершиной рёбра
        for(int i = 0; i < size; ++i)
            for(list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if((*j).v2 == index) {
                    delete (*j).edge;
                    edgeList[i].erase(j);
                    ++deleted;
                    //Стираем симметричное ребро
                    if(directed == false)
                        for(list<Node>::iterator k = edgeList[index].begin(); k != edgeList[index].end(); ++k)
                            if((*k).v2 == i) {
                                edgeList[index].erase(k);
                                break;
							}
							break;
				}
	    if (directed==true){
			for(list<Node>::iterator z = edgeList[index].begin(); z != edgeList[index].end(); ++z){  
				delete (*z).edge; 
				deleted++;
			}
		}	
        return deleted;
    }
    //Проверка и получение
    bool hasEdge(int v1, int v2) {
        int size = edgeList.size(); //Число вершин
        //Неверный номер вершины
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			return false;
        //Петля
        if(v1 == v2) 
			return false;
        for(list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if((*j).v2 == v2)
                return true;
        return false;
    }
    EdgeT* getEdge(int v1, int v2) {
        int size = edgeList.size(); //Число вершин
        //Неверный номер вершины
        if(v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) 
			throw 1;
        //Петля
        if(v1 == v2) 
			throw 1;
        for(list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if((*j).v2 == v2)
                return (*j).edge;
        throw 1;
    }

    //Итератор рёбер
    
	class EdgeIterator {
        int curI;
        typename list<typename GraphListForm<EdgeT>::Node>::iterator curJ;
        bool end;
        GraphListForm *f;
    public:
        EdgeIterator(GraphListForm *f) {
            this->f=f;
            begin();       
		}
        bool begin() {
            for(curI = 0; curI < f->edgeList.size(); ++curI)
                for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
                    if((*curJ).edge) {
                        end = false;
                        return true;                    
					}
            end = true;
            return false;        
		}
        bool onEnd() { 
			return end; 
		}
		bool toend(){
			 typename list<typename GraphListForm<EdgeT>::Node>::iterator prev_curJ;
			 for(curI = 0; curI < f->edgeList.size(); ++curI)
                for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
					if ((*curJ).edge)
						prev_curJ=curJ;
			 if (!(*prev_curJ).edge){
				 end=true;
				 return false;
			 }
			 else{
				 end=false;
				 curJ=prev_curJ;
				 return true;
			 };  
		};
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curI < f->edgeList.size()) {
                while(curJ != f->edgeList[curI].end()) {
                    if((*curJ).edge && (f->directed || !f->directed && (*curJ).v2 > curI))
                        return true;
                    ++curJ;                
				}
                ++curI;
                if(curI < f->edgeList.size())
                    curJ = f->edgeList[curI].begin();
            }
            end = true;
            return true;        
		}
        EdgeT* getEdge() {
            if(end)
                throw 1;
            return (*curJ).edge;
        }
    };

    //Итератор исходящих рёбер
    
	class OutputEdgeIterator {
        int curI;
        typename list<typename GraphListForm<EdgeT>::Node>::iterator curJ;
        bool end;
        GraphListForm *f;
    public:
        OutputEdgeIterator(GraphListForm *f, int vIndex) {
            this->f = f;
            curI = vIndex;
            begin();
        }
        bool begin() {
            for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
                if((*curJ).edge) {
                    end = false;
                    return true;                
				}
            end = true;
            return false;        
		}
        bool onEnd() { 
			return end; 
		}
		bool toend(){
			 typename list<typename GraphListForm<EdgeT>::Node>::iterator prev_curJ;
			 for(curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
				 if ((*curJ).edge)
					 prev_curJ=curJ;
			 if (!(*prev_curJ).edge){
				 end=true;
				 return false;
			 }
			 else{
				 end=false;
				 curJ=prev_curJ;
				 return true;
			 };  
		}
        bool next() {
            if(end)
                return false;
            ++curJ;
            while(curJ != f->edgeList[curI].end()) {
                if((*curJ).edge)
                    return true;
                ++curJ;            
			}
            end = true;
            return true;
        }
        EdgeT* getEdge() {
            if(end)
                throw 1;
            return (*curJ).edge;        
		}    
	};
};


//===============

//Граф

template <class VertexT, class EdgeT>
class Graph {
    friend class VertexIterator;
    vector<VertexT*> vertexVector;//Вектор вставленных
    GraphForm<EdgeT> *data;       //Данные графа
    bool directed;                //Направленный граф
    bool dense;                   //М-граф
    int edgeCounter;              //Число рёбер

    //Получение индекса вершины по дескриптору
	int getIndexFromName(string n){
		int index=0;
		for(int i=0; i<vertexVector.size(); ++i){
			if (vertexVector[index]->getName()==n)
				break;
			++index;
		}
		if(index == vertexVector.size())
            throw "Данной вершины нет в графе";
        return index;
	};

public:
    //Конструкторы:
    //Пустой L граф
    Graph() {
        data = new GraphListForm<EdgeT>(false);
        directed = false;
        dense = false;
        edgeCounter = 0;
    }
    //Заданы кол-во вершин, направленность, форма
    Graph(int vertexCount, bool directed, bool dense) {
        if(dense)
            data = new GraphMatrixForm<EdgeT>(directed);//!!!!!!!!!!!!1
        else
            data = new GraphListForm<EdgeT>(directed);//!!!!!!!!!!!!!!!!!!!!
        //Создаём вершины и помещаем в структуру
        for(int i = 0; i < vertexCount; ++i)
            data->insertVertex(i);
        for(int i = 0; i < vertexCount; ++i) {
            VertexT *v = new VertexT();
			v->setName(i_to_s(sz));
			sz++;
            vertexVector.push_back(v);
            data->insertVertex(i);
        }
        this->directed = directed;
        this->dense = dense;
        edgeCounter = 0;
    }
    //+ задано кол-во случайных рёбер
    Graph(int vertexCount, int edgeCount, bool directed, bool dense) {
        if(dense)
            data = new GraphMatrixForm<EdgeT>(directed);
        else
            data = new GraphListForm<EdgeT>(directed);
        //Создаём вершины и помещаем в структуру
        for(int i = 0; i < vertexCount; ++i)
            data->insertVertex(i);
        for(int i = 0; i < vertexCount; ++i) {
            VertexT *v = new VertexT(); 
			v->setName(i_to_s(sz));
			sz++;
            vertexVector.push_back(v);
            data->insertVertex(i);
        } 
        this->directed = directed;
        this->dense = dense;
        edgeCounter = 0;

        //Вставляем рёбра
        if(edgeCount <= 0) return;
        //Корректируем число рёбер, чтобы не превышало максимумы
        if(directed) {
            if(edgeCount > vertexCount * (vertexCount - 1))
                edgeCount = vertexCount * (vertexCount - 1);
            srand(time(0));
        } else {
            if(edgeCount > (vertexCount * (vertexCount - 1)) / 2)
                edgeCount = (vertexCount * (vertexCount - 1)) / 2;
        }
        //Вставляем
        int v1, v2;

        while(edgeCounter < edgeCount) {
            v1 = rand() % vertexCount;
            v2 = rand() % vertexCount;
            if(v1 == v2)
                continue;
            if(data->hasEdge(v1, v2))
                continue;

            EdgeT *t = new EdgeT(vertexVector[v1], vertexVector[v2]);
            data->insertEdge(v1, v2, t);
			t->setWeight((rand() % 9) +1);
            if(directed == false)
                data->insertEdge(v2, v1, t);
            ++edgeCounter;
        }
    }
    Graph(const Graph<VertexT, EdgeT> &G) {
        delete data;
        if(G.isDense())
            data = new GraphMatrixForm<EdgeT>(*(G.data));
        else
            data = new GraphListForm<EdgeT>(*(G.data));
        directed = G.directed;
        dense = G.dense;
    }
    ~Graph() {
		while (vertexVector.size()!=0)
			deleteVertex(vertexVector.size()-1);
        delete data;
    }
    //Информация о графе
    //Число вершин, рёбер, насыщенность
    int getVertexCount() { 
		return vertexVector.size(); 
	}
    int getEdgeCount() { 
		return edgeCounter; 
	}
    double getRatio() {
        int max = vertexVector.size() * (vertexVector.size() - 1);
        if(!directed)
            max /= 2;
        return (double)edgeCounter / (double)max;
    }
    //Тип графа
    bool isDirected() { 
		return directed; 
	}
    bool isDense() { 
		return dense; 
	}
    //Преобразования
    void toDenseGraph() {
        //Создаём структуру хранения М-графа
		GraphForm<EdgeT> *newData = new GraphMatrixForm<EdgeT>(this->directed);

        //Создаём вершины
        for(int i = 0; i < vertexVector.size(); ++i)
            newData->insertVertex(i);

        //Переносим рёбра
        for(int i = 0; i < vertexVector.size(); ++i)
            for(int j = 0; j < vertexVector.size(); ++j)
                if(data->hasEdge(i, j))
                    newData->insertEdge(i, j, data->getEdge(i, j));
        delete data;
        data = newData;
        dense = true;
    }
    void toSparseGraph() {
        //Создаём структуру хранения L-графа
		GraphForm<EdgeT> *newData = new GraphListForm<EdgeT>(this->directed);

        //Создаём вершины
        for(int i = 0; i < vertexVector.size(); ++i)
            newData->insertVertex(i);

        //Переносим рёбра
        for(int i = 0; i < vertexVector.size(); ++i)
            for(int j = 0; j < vertexVector.size(); ++j)
                if(data->hasEdge(i, j))
                    newData->insertEdge(i, j, data->getEdge(i, j));
        delete data;
        data = newData;
        dense = false;
    }


    //Данные
    //Вершины
    VertexT* insertVertex() {
        VertexT *v = new VertexT;
        if(data->insertVertex(vertexVector.size()) == false)
            throw 1;
		v->setName(i_to_s(sz));
		sz++;
        vertexVector.push_back(v);
        return v;
    }
    VertexT* insertVertex(int index) {
        VertexT *v = new VertexT;
        if(data->insertVertex(index) == false)
            throw 1;
        vertexVector.insert(vertexVector.begin() + index, v);
        return v;
    }
    bool deleteVertex(VertexT *v) {
        int index = getIndex(v);
        edgeCounter -= data->deleteEdgesFromVertex(index, directed);
        if(data->deleteVertex(index)) {
            vertexVector.erase(vertexVector.begin() + index);
            return true;
        }
        return false;
    }
	bool deleteVertex(int index) {
        edgeCounter -= data->deleteEdgesFromVertex(index, directed);
        if(data->deleteVertex(index)) {
            vertexVector.erase(vertexVector.begin() + index);
            return true;
        }
        return false;
    }
    bool deleteVertex(string n) {
		VertexT *v=new VertexT;
		bool rez;
		try{
			v=getVertexFromName(n);
		}
		catch(char*){
			return false;
		}
		rez=deleteVertex(v);
		return rez;
    }
    VertexT* getVertex(int index) {
        if(index < 0 || index >= vertexVector.size())
            throw "Данной вершины нет в графе";
        return vertexVector[index];
    }
	VertexT* getVertexFromName(string n){
		int i;
		for(i=0; i<vertexVector.size(); i++)
			if (vertexVector[i]->getName()==n)
				return vertexVector[i];
		if (i==vertexVector.size())
			throw "Данной вершины нет в графе";
	};
	int getIndex(VertexT *v) {
		int index = 0;
        for(int i = 0; i < vertexVector.size(); ++i) {
            if(vertexVector[index] == v)
                break;
            ++index;
        }
        if(index == vertexVector.size())
            throw "Данной вершины нет в графе";
        return index;
    }
    //Рёбра
	void insertEdge(string v1, string v2){
		VertexT* _v1=new VertexT;
		VertexT* _v2=new VertexT;
		try{
			_v1=getVertexFromName(v1);
		}
		catch(char*){
			throw "Неверное имя вершины 1";
		};
		try{
			_v2=getVertexFromName(v2);
		}
		catch(char*){
			throw "Неверное имя вершины 2";
		}
		try{
			insertEdge(_v1, _v2);
		}
		catch (char* Error){
			throw "Петля недопустима, или ребро уже есть";
		}
		return;
	};
    EdgeT* insertEdge(VertexT *v1, VertexT *v2) {
        EdgeT *e = new EdgeT(v1, v2);
        if(!data->insertEdge(getIndex(v1), getIndex(v2), e))
			throw "Петля недопустима, или ребро уже есть";
        if(directed == false)
            data->insertEdge(getIndex(v2), getIndex(v1), e);
        ++edgeCounter;
        return e;
    }
	bool deleteEdge(string v1, string v2){
		VertexT* _v1=new VertexT;
		VertexT* _v2=new VertexT;
		try{
			_v1=getVertexFromName(v1);
		}
		catch(char*){
			throw "Неверный номер вершины 1";
		};
		try{
			_v2=getVertexFromName(v2);
		}
		catch(char*){
			throw "Неверный номер вершины 2";
		}
		if(!deleteEdge(_v1, _v2))
			return false;
		
		return true;
	};
    bool deleteEdge(VertexT *v1, VertexT *v2) {
        if(data->deleteEdge(getIndex(v1), getIndex(v2))) {
            --edgeCounter;
            if(directed == false)
                data->deleteEdge(getIndex(v2), getIndex(v1));
            return true;
        } else
            return false;
    }
    bool hasEdge(VertexT *v1, VertexT *v2) {
        int ind1, ind2;
        try { 
			ind1 = getIndex(v1); 
		} 
		catch(int &a) { 
			return false; 
		}
        try {
			ind2 = getIndex(v2); 
		}
		catch(int &a) 
		{ 
			return false; 
		}
        return data->hasEdge(getIndex(v1), getIndex(v2));
    }
    bool hasEdge(int v1, int v2) {
        if(v1 < 0 || v1 >= vertexVector.size()) 
			return false;
        if(v2 < 0 || v2 >= vertexVector.size()) 
			return false;
        return data->hasEdge(v1, v2);
    }
    EdgeT* getEdge(VertexT *v1, VertexT *v2) {
		EdgeT* e;
		try{
			e=data->getEdge(getIndex(v1), getIndex(v2));
		}
		catch(int){
			throw "        False";
		}
        return e;
    }
	bool write_data_edge(string _v1, string _v2, int data){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			return false;
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			return false;
		}
		e=getEdge(v1, v2);
		e->setData(data);
		return true;
	};
	bool write_weight_edge(string _v1, string _v2, int w){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			return false;
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			return false;
		}
		e=getEdge(v1, v2);
		e->setWeight(w);
		return true;
	};
	int read_data_edge(string _v1, string _v2){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			throw "        False";
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			throw "        False";
		}
		e=getEdge(v1, v2);
		if (e->isDataSet())
			return e->getData();
		else
			throw "        Null";
	};
	int read_weight_edge(string _v1, string _v2){
		EdgeT* e;
		VertexT *v1, *v2;
		try{
			v1=getVertexFromName(_v1);
		}
		catch(char*){
			throw "        False";
		}
		try{
			v2=getVertexFromName(_v2);
		}
		catch(char*){
			throw "        False";
		}
		e=getEdge(v1, v2);
		if (e->isWeightSet())
			return e->getWeight();
		else
			throw "        Null";
	};
	void print_graph() {
		int i, j;
		VertexT* v;
		EdgeT* e;
		if (isDense()) {
			cout << " |";
			for (i = 0; i < getVertexCount(); i++) {
				v = getVertex(i);
				cout << setw(4) << v->getName();
			}
			cout << endl;
			cout << "--|";
			for (i = 0; i <= 4 * getVertexCount(); i++)
				cout << "-";
			cout << endl;
			for (i = 0; i < getVertexCount(); i++) {
				v = getVertex(i);
				cout << " " << v->getName() << "|";
				for (j = 0; j < getVertexCount(); j++) {
					if (hasEdge(i, j)) {
						e = getEdge(getVertex(i), getVertex(j));
						cout << setw(4) << 1;
					}
					else {
						cout << setw(4) << "0";
					}
				}
				cout << endl;
			}
		}
		else {
			GraphListForm<EdgeT>* form = static_cast<GraphListForm<EdgeT>*>(data);
			for (i = 0; i < getVertexCount(); i++) {
				v = getVertex(i);
				cout << "*" << v->getName() << " ->";
				list<typename GraphListForm<EdgeT>::Node> l = form->edgeList[i];
				for (typename list<typename GraphListForm<EdgeT>::Node>::iterator it = l.begin(); it != l.end(); it++) {
					if ((*it).edge != NULL) {
						cout << getVertex((*it).v2)->getName() << "->";
					}
				}
				cout << endl;
			}
		}
	};

		int read_data_vertex(string v1){
			VertexT *v;
			try
			{
				v=getVertexFromName(v1);
			}
			catch(char*){
				throw "        False";
			};
			int d;
			d=v->getData();
			if (v->dataSet!=false)
				return d;
			else
				throw "        Null";
		};
		bool write_data_vertex(string v1, int d){
			VertexT *v;
			try
			{
				v=getVertexFromName(v1);
			}
			catch(char*){
				return false;
			};
			v->setData(d);
			return true;
		};
		bool write_name_vertex(string v1, string str){
			VertexT *v;
			try
			{
				v=getVertexFromName(v1);
			}
			catch(char*){
				return false;
			};
			v->setName(str);
			return true;
		};


    //Вложенные классы
    
	//Итератор вершин
    
	class VertexIterator {
        Graph<VertexT, EdgeT> *graph; //Обрабатываемый граф
        bool end;
        int current;
    public:
        VertexIterator(Graph<VertexT, EdgeT> &g) {
            graph = &g;
            begin();
        }
        //Установка в начало
        void begin() {
            if(graph->vertexVector.size() == 0) {
                end = true;
                return;
            }
            current = 0;
            end = false;
			return;
        }
        //Сдвиг на следующий элемент
        void operator++() {
            if(end){
                current=-1;
				return;
			}
            current++;
            if(graph->vertexVector.size()-1 == current)
                end = true;
            return;
        };
		void toend(){
			current=graph->vertexVector.size()-1;
			end=true;
			return;
		};
        //Проверка на выход
        bool onEnd() {
            return end;
        }
        //Получение вершины
        VertexT* operator*() {
            if(current >= graph->vertexVector.size() || current==-1)
                throw 1;
            return graph->vertexVector[current];
        }
		string read_vertex() {
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			string d;
			d=v->getName();
			if (v->nameSet!=false)
				return d;
			else
				throw "        Null";
        }
		int read_data_vertex(){
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			int d;
			d=v->getData();
			if (v->dataSet!=false)
				return d;
			else
				throw "        Null";
		};
		void write_data_vertex(int d){
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			v->setData(d);
			return;
		};
		void write_name_vertex(string str){
			VertexT *v;
			try
			{
				v=operator*();
			}
			catch(int){
				throw "        False";
			};
			v->setName(str);
			return;
		};
		
	};

    //Итератор рёбер

    class EdgeIterator {
        Graph<VertexT, EdgeT> *graph; //Обрабатываемый граф
        typename GraphMatrixForm<EdgeT>::EdgeIterator *mIt;
        typename GraphListForm<EdgeT>::EdgeIterator *lIt;
        bool useM;
    public:
		int curV1, curV2;
		bool end;
        EdgeIterator(Graph<VertexT, EdgeT> &g) {
            graph = &g;
            if(graph->isDense()) {
                mIt = new GraphMatrixForm<EdgeT>::EdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data));
                useM = true;
            } else {
                lIt = new GraphListForm<EdgeT>::EdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data));
                useM = false;
            }
            begin();
        }
        //Установка в начало
        bool begin() {
            if(useM) return 
				mIt->begin();
            else 
				return lIt->begin();
        }
        //Сдвиг на следующий элемент
        bool operator++() {
            if(useM)
                return mIt->next();
            else
                return lIt->next();
        }
        //Проверка на выход
        bool onEnd() {
            //return end;
            if(useM)
                return mIt->onEnd();
            else
                return lIt->onEnd();
        }
		bool toend(){
			if(useM)
				return mIt->toend();
			else
				return lIt->toend();
		}
        //Получение ребра
        EdgeT* operator*() {
			if (onEnd())
				throw "        Null";
            if(useM)
                return mIt->getEdge();
            else
                return lIt->getEdge();
        }
		string read_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			VertexT *_v1, *_v2;
			int w;
			string str1, str2, str3, str4;
			_v1=e->getVertex1();
			_v2=e->getVertex2();
			if (_v1->nameSet==false)
				str1="Null";
			else
				str1=_v1->getName();
			if (_v2->nameSet==false)
				str2="Null";
			else
				str2=_v2->getName();
			if (e->isWeightSet()==false)
				str3="Null";
			else
				str3=i_to_s(e->getWeight());
			str4="        Исходящая вершина: "+str1+". Входящая вершина: "+str2+". Вес ребра: "+str3;
			return str4;
		}
		int read_data_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			int a;
			if (e->isDataSet())
				return e->getData();
			else
				throw "        Null";
		};
		bool write_data_edge(int d){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setData(d);
			return true;
		};
		bool write_weight_edge(int w){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setWeight(w);
			return true;
		}
    };

    //Итератор исходящих рёбер

    class OutputEdgeIterator {
        Graph<VertexT, EdgeT> *graph; //Обрабатываемый граф
        typename GraphMatrixForm<EdgeT>::OutputEdgeIterator *mIt;
        typename GraphListForm<EdgeT>::OutputEdgeIterator *lIt;
        bool useM;
    public:
		int curV1, curV2;
		bool end;
        OutputEdgeIterator(Graph<VertexT, EdgeT> &g, VertexT &v) {
            graph = &g;
            curV1 = graph->getIndex(&v);
            if(graph->isDense()) {
                mIt = new GraphMatrixForm<EdgeT>::OutputEdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data), curV1);
                useM = true;
            } else {
                lIt = new GraphListForm<EdgeT>::OutputEdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data), curV1);
                useM = false;
            }
            begin();
        }
        //Установка в начало
        bool begin() {
            if(useM)
                return mIt->begin();
            else
                return lIt->begin();
        }
        //Сдвиг на следующий элемент
        bool operator++() {
            if(useM)
                return mIt->next();
            else
                return lIt->next();
        }
		bool toend(){
			if(useM)
				return mIt->toend();
			else
				return lIt->toend();
		}
        //Проверка на выход
        bool onEnd() {
            if(useM)
                return mIt->onEnd();
            else
                return lIt->onEnd();
        }
        //Получение ребра
        EdgeT* operator*() {
			if (onEnd())
				throw "        False";
            if(useM)
                return mIt->getEdge();
            else
                return lIt->getEdge();
        }
		string read_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			VertexT *_v1, *_v2;
			int w;
			string str1, str2, str3, str4;
			_v1=e->getVertex1();
			_v2=e->getVertex2();
			if (_v1->nameSet==false)
				str1="Null";
			else
				str1=_v1->getName();
			if (_v2->nameSet==false)
				str2="Null";
			else
				str2=_v2->getName();
			if (e->isWeightSet()==false)
				str3="Null";
			else
				str3=i_to_s(e->getWeight());
			str4="        Исходящая вершина: "+str1+". Входящая вершина: "+str2+". Вес ребра: "+str3;
			return str4;
		}
		int read_data_edge(){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				throw "        False";
			}
			int a;
			if (e->isDataSet())
				return e->getData();
			else
				throw "        Null";
		};
		bool write_data_edge(int d){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setData(d);
			return true;
		};
		bool write_weight_edge(int w){
			EdgeT* e;
			try{
				e=operator*();
			}
			catch(char* Error){
				return false;
			}
			e->setWeight(w);
			return true;
		}
    };

	//template<class VertexT, class EdgeT>
	class Task1 {
		int** Adj;			//матрица смежных вершин
		int* c;			//ход, на котором посещается вершина
		int n;				//кол верш в графе	
		int v0 = 0;		//начальная вершина
		int cd = 0;
		int* exit;		// ответ
		Graph<VertexT, EdgeT>* graph; //Обрабатываемый граф 				

	public:
		typename Graph<VertexT, EdgeT>::OutputEdgeIterator* out;

		Task1(Graph<VertexT, EdgeT>& g) {
			graph = &g;
		}
		Task1(const Graph<VertexT, EdgeT>& T) {
			graph = T.graph;
		}
		~Task1() {
			delete с, Adj, exit;
		}
		bool restart(int v0, int n) {
			
			this->n = n;
			this->v0 = v0;
			cd = graph->getVertexCount();

			c = new int [cd];
			exit = new int[cd];
			Adj = new int* [cd];

			for (int i = 0; i < cd; i++) {
				Adj[i] = new int[cd];
					c[i] = -1;
			}
			c[v0] = v0;
			exit[0] = v0;
			search();
			if (!DFS_Visit(1)) return false;

			result(n);
			return true;
		}
		int DFS_Visit(int k) {

			int v, q1 = 0;
			for (v = 0; v < cd && !q1; v++)
			{
				if (Adj[exit[k - 1]][v] > 0)
				{
					if (k == n && v == v0) q1 = 1; // show(n);
					else if (c[v] == -1)
					{
						c[v] = k; exit[k] = v;
						q1 = DFS_Visit(k + 1);
						if (!q1) c[v] = -1;
					}
					else continue;
				}
			}
			return q1;
		}

		void search() {
			int vv1, vv2;
			for (int u = 0; u < cd; u++) {
				try {
					out = new Graph<VertexT, EdgeT>::OutputEdgeIterator(*graph, *graph->getVertex(u));
				}
				catch (char*) {
					continue;
				}
				if (!out->begin()) {
					continue;
				}

				for (int j = 0; j < cd; j++) {
					Adj[u][j] = 0;
					if (graph->hasEdge(u, j)) {
						EdgeT* e = graph->getEdge(graph->getVertex(u), graph->getVertex(j));
						Adj[u][j] = 1;
					}
				}
			}
		}
		void result(int n) {

			for (int i = 0; i < n; i++) {
				cout << exit[i] << " -> ";
			}
			cout << v0;
		}
		//-----------------------------------------------------

	};
};

struct tmpE
{
	int v1, v2;
	int w;
	int s;
};

typedef double TElem;
typedef vector<TElem> TVec;
typedef vector<TVec> TMatrix;
typedef vector<int> TIntVec;
typedef vector<TIntVec> TIntMatrix;
TElem inf = numeric_limits<TElem>::infinity();

//Процедура печати матрицы:
template <class Elem>
void PrintMatrix(const vector<vector<Elem>>& A)
{
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < A[i].size(); j++)
		{
			cout << setw(3) << A[i][j];
		}
		cout << endl;
	}
}

template <class VertexT, class EdgeT>
class Task2 {
private:
	Graph<VertexT, EdgeT>* graph;   //Обрабатываемый граф
	int n;							//Число вершин графа
	stringstream res;               //Результат

	struct TDiam
	{
		TElem max;	//Значение диаметра
		int i, j;	//Крайние вершины
	};
	void GenerateWeightMatrix(TMatrix& W)
	{
		//Заполнение матрицы весов весами рёбер графа:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (graph->hasEdge(i, j))
				{
					EdgeT* e = graph->getEdge(graph->getVertex(i), graph->getVertex(j));
					if (e->isWeightSet())
						W[i][j] = e->getWeight();
					else
						throw exception("Не установлен вес ребра");
				}
				else
				{
					W[i][j] = inf;
				}
			}
		}
	}
	void Floyd(const TMatrix& W, TMatrix& D, TIntMatrix& P)
	{
		//Инициализация матрицы весов кратчайших путей:
		for (int i = 0; i < n; i++)
		{
			D[i][i] = 0;
			for (int j = 0; j < n; j++)
			{
				if (i != j)
				{
					D[i][j] = W[i][j];
				}
			}
		}
		//Инициализация матрицы предшественников:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (i != j && W[i][j] != inf)
				{
					P[i][j] = i;
				}
				else
				{
					P[i][j] = -1;
				}
			}
		}
		//Алгоритм Флойда:
		for (int k = 0; k < n; k++)
		{
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					if (D[i][j] > D[i][k] + D[k][j])
					{
						D[i][j] = D[i][k] + D[k][j];
						P[i][j] = P[k][j];
					}
				}
			}
		}
	}
	TDiam FindDiam(const TMatrix& D)
	{
		TDiam diam = { -inf, -1, -1 };
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (D[i][j] != inf && D[i][j] > diam.max)
				{
					diam.max = D[i][j];
					diam.i = i;
					diam.j = j;
				}
			}
		}
		return diam;
	}
	void PrintPath(const TIntMatrix& P, int s, int f)
	{
		if (f == s)
		{
			res << f << " -> ";
		}
		else
		{
			PrintPath(P, s, P[s][f]);
			res << f << " -> ";
		}
	}
public:
	//Конструкторы-деструкторы
	Task2(Graph<VertexT, EdgeT>* g) : graph(g)
	{
		restart();
	}
	Task2(const Task2<VertexT, EdgeT>& t1) : graph(t1.g), n(t1.n) { }
	void set(Graph<VertexT, EdgeT>* g)
	{
		graph = g;
		restart();
	}
	void restart()
	{
		res.str("");
		n = graph->getVertexCount();
		if ((graph->getEdgeCount()) < 1)
			throw exception("В графе нет рёбер");
		TMatrix W(n, TVec(n));			//Матрица весов
		GenerateWeightMatrix(W);		//Генерация матрицы весов
		TMatrix D(n, TVec(n));			//Матрица весов кратчайших путей
		TIntMatrix P(n, TIntVec(n));	//Матрица предшественников
		Floyd(W, D, P);
		TDiam diam = FindDiam(D);		//Поиск диаметра графа
		res << "Диаметр графа = " << diam.max << endl;
		if (diam.max < inf)
		{
			res << "Путь, соответствующий диаметру:" << endl;
			PrintPath(P, diam.i, diam.j);
			res << endl;
		}
	}
	string result()
	{
		return res.str();
	}
};


class QueueNode{
public:
	float PriorityWeight;
	int Parent;
	int QueIndex; 
	QueueNode(float w, int p){
		PriorityWeight=w;
		Parent=p;
	}
	
};   


class MyObj{
public:
	bool operator()(QueueNode* w1, QueueNode* w2){
		if (w1->PriorityWeight>w2->PriorityWeight)
			return true;
		else
			return false;
	};
};

class MinEdge{	
public: 	
	int Pr;
	int V;
	int Weight;
	MinEdge(int v, int w, int p){
		Pr = p;
		V = v;
		Weight=w;
	};
};

//==============

int enter_menu(){
	int menu;
	cin>>menu;
	while (!cin.good()){
		cout<<"Неверный формат данных!"<<endl;
		cout<<"Введите верный пункт меню: ";
		cin.clear();
		_flushall();
		cin>>menu;
	}
	return menu;
}

void main()
{
	int menu;
	Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>> *graph;
	Vertex<string, int> *vertex;
	Edge<Vertex<string, int>, int, int> *edge;

	Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator *vIt;
    Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator *eIt;
    Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::OutputEdgeIterator *oIt;
	Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::Task1* Tt1;

	setlocale (0, "Rus");
	//system("mode 110,50");
	cout<<"Выберите пункт меню: "<<endl;
	cout<<"1 - Создать граф с V вершинами, без ребер, типа D, формы пред-ставления F"<<endl;
	cout<<"2 - Создать пустой L-граф с нулевым числом вершин и рёбер"<<endl;
	cout<<"3 - создать граф с V вершинами, с E случайными ребрами, типа D , формы F"<<endl;
	cout<<"-1 - Выход"<<endl;
	bool flag1=false;
	while (!flag1){
		cout<<">>";
		menu=enter_menu();
		switch (menu){
		case 1:{
			int v;
			bool directed, dense;
			string dir, den;
			cout<<"Введите количество вершин: ";
			cin>>v;
			cout<<"Укажите, ориентированный ли граф: ";
			cin>>dir;
			cout<<"Укажите форму графа (0 - матрица, 1 - список): ";
			cin>>den;
			if (dir=="true")
				directed=true;
			else
				directed=false;
			if (den=="true")
				dense=true;
			else
				dense=false;
			graph=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(v, directed, dense);
			cout<<"Граф создан"<<endl;
			cout<<"===================================="<<endl;
			flag1=true;
				};
			break;
		case 2:{
			graph=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>();
			cout<<"Граф создан"<<endl;
			cout<<"===================================="<<endl;
			flag1=true;
				};
			break;
		case 3:{
			int v, e;
			bool directed, dense;
			string dir, den;
			cout<<"Введите количество вершин: ";
			cin>>v;
		    cout<<"Введите количество рёбер: ";
			cin>>e;
			cout<<"Укажите, ориентированный ли граф: ";
			cin>>dir;
			cout<<"Укажите форму графа (1 - матрица, 0 - список): ";
			cin>>den;
			if (dir=="true" || dir=="True")
				directed=true;
			else
				directed=false;
			if (den=="true" || dir=="True")
				dense=true;
			else
				dense=false;
			graph=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(v, e, directed, dense);
			cout<<"Граф создан"<<endl;
			cout<<"===================================="<<endl;
			flag1=true;
				};
			break;
		case -1: {
			return;
				 };
			break;
		default: {
			cout<<"Неверно выбран пункт меню!"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите верный пункт: ";
				 };
		}
	}
	cout<<"Выберите пункт меню: "<<endl;
	cout << "=================================================================" << endl;
	cout<<"0 - Распечатать граф"<<endl;
	cout<<"1 - Опрос числа вершин в графе"<<endl;
	cout<<"2 - Опрос числа рёбер в графе"<<endl;
	cout<<"3 - Опрос типа графа"<<endl;
	cout<<"4 - Опрос формы представления графа"<<endl;
	cout<<"5 - Опрос коэффициента насыщенности графа"<<endl;
	cout << "=================================================================" << endl;
	cout<<"6 - Преобразовать граф к L-форме"<<endl;
	cout<<"7 - Преобразовать граф к M-форме"<<endl;
	cout << "=================================================================" << endl;
	cout<<"8 - Добавить вершину к графу"<<endl;
	cout<<"9 - Удалить вершину из графа"<<endl;
	cout << "=================================================================" << endl;
	cout<<"10 - Вставить ребро в граф"<<endl;
	cout<<"11 - Удалить ребро из графа"<<endl;
	cout << "=================================================================" << endl;
	cout<<"12 - Операции над вершиной"<<endl;
	cout<<"13 - Итератор вершин"<<endl;
	cout<<"14 - Итератор рёбер"<<endl;
	cout<<"15 - Итератор исходящих рёбер"<<endl;
	cout<<"16 - Операции над ребром"<<endl;
	cout << "=================================================================" << endl;
	cout<<"20 - Поиск простого цикла из заданной вершины"<<endl;
	cout<<"21 - Определение диаметра и пути, соотв. диаметру (алгоритм Флойда)"<<endl;
	cout << "=================================================================" << endl;
	cout<<"-1 - Выход"<<endl;
	while (2>1){
		cout<<">>";
		menu=enter_menu();
		switch (menu){
		case 0:{
			cout<<"Печать графа: "<<endl;
			graph->print_graph();
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 1:{
			cout<<"Число вершин в графе: "<<graph->getVertexCount()<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 2:{
			cout<<"Число рёбер в графе: "<<graph->getEdgeCount()<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 3:{
			cout<<"Тип графа: ";
			if (graph->isDirected()==true)
				cout<<"ориентированный"<<endl;
			else
				cout<<"неориентированный"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 4:{
			cout<<"Форма предстваления графа: ";
			if (graph->isDense()==true)
				cout<<"M-граф"<<endl;
			else
				cout<<"L-граф"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 5:{
			cout<<"Коэффициент насыщенности: "<<graph->getRatio()<<endl;
			   };
			break;
		case 6:{
			graph->toSparseGraph();
			cout<<"Граф преобразован"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 7:{
			graph->toDenseGraph();
			cout<<"Граф преобразован"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 8:{
			try{
				graph->insertVertex();
			}
			catch (int Error){
				cout<<Error;
			};
			cout<<"Вершина добавлена в граф"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 9:{
			string v1;
			cout<<"Введите удаляемую вершину: ";
			cin>>v1;
			if (graph->deleteVertex(v1)==true)
				cout<<"Вершина удалена"<<endl;
			else
				cout<<"Данной вершины нет в графе"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
			   };
			break;
		case 10:{
			string v1, v2;
			cout<<"Введите вершину, из которой выходит ребро: ";
			cin>>v1;
			cout<<"Введите вершину, в которую входит ребро: ";
			cin>>v2;
			try{
				graph->insertEdge(v1, v2);
			}
			catch(char* Error){
				cout<<Error<<endl;
				cout<<"===================================="<<endl;
			    cout<<"Введите пункт меню: ";
				break;
			};
			cout<<"Ребро добавлено в граф"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
				};
			break;
		case 11:{
			string v1, v2;
			bool rez;
			cout<<"Введите вершину, из которой выходит ребро: ";
			cin>>v1;
			cout<<"Введите вершину, в которую входит ребро: ";
			cin>>v2;
			try{
				rez=graph->deleteEdge(v1, v2);
			}
			catch(char* Error){
				cout<<Error<<endl;
				cout<<"===================================="<<endl;
			    cout<<"Введите пункт меню: ";
				break;
			};
			if (rez)
				cout<<"Ребро удалено"<<endl;
			else
				cout<<"Ребра, проходящего через заданные вершины, не существует"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
				};
			break;
		case 12:{
			bool flag=false;
			cout<<"Операции над вершиной:"<<endl;
			cout<<"        1 - Прочитать данные о вершине"<<endl;
			cout<<"        2 - Записать данные о вершине"<<endl;
			cout<<"        3 - Записать новое имя вершины"<<endl;
			cout<<"        -1 -Выход из меню"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
					case 1:{
						int cur;
						string v;
						cout<<"        Введите имя вершины: ";
						cin>>v;
						try{
							cur=graph->read_data_vertex(v);
						}
						catch (char* Error){
							cout<<Error<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        Введите пункт меню: ";
							break;
						}
						cout<<"        Данные вершины: "<<cur<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите пункт меню: ";
						   };
						break;
					case 2:{
						int cur;
						string v;
						bool rez;
						cout<<"        Введите имя вершины: ";
						cin>>v;
						cout<<"        Введите данные: ";
						cin>>cur;
						rez=graph->write_data_vertex(v, cur);
						if (!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        Введите пункт меню: ";
							break;
						}
						cout<<"        Данные о вершине записаны"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите пункт меню: ";
						   };
						break;
					case 3:{
						bool rez;
						string v, str;
						cout<<"        Введите старое имя вершины: ";
						cin>>v;
						cout<<"        Введите новое имя: ";
						cin>>str;
						rez=graph->write_name_vertex(v, str);
						if(!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        Введите пункт меню: ";
							break;
						}
						cout<<"        Имя вершины перезаписано"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите пункт меню: ";
						   };
						break;
					case -1:{
						flag=true;
						cout<<"===================================="<<endl;
							};
						break;
					default: {
						cout<<"        ===================================="<<endl;
						cout<<"        Введите верный пункт: "; 
							 };
						break; 
				};
			};
				}
			break;
		case 13:{
			bool flag=false;
			vIt=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator(*graph);
			cout<<"Итератор вершин:"<<endl;
			cout<<"        1 - Установить итератор вершин на начальную вершину"<<endl;
			cout<<"        2 - Установить итератор вершин на конечную вершину"<<endl;
			cout<<"        3 - Переход к следующей позиции"<<endl;
			cout<<"        4 - Прочитать вершину по текущей позиции итератора"<<endl;
			cout<<"        5 - Прочитать данные вершины по текущей позиции итератора"<<endl;
			cout<<"        6 - Записать данные о вершины по текущей позиции итератора"<<endl;
			cout<<"        7 - Записать имя вершины по текущей позиции итератора"<<endl;
			cout<<"        -1 -Выход из меню итератора"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
				case 1:{
					vIt->begin();
			        cout<<"        Итератор вершин графа установлен на первую вершину"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 2:{
			        vIt->toend();
			        cout<<"        Итератор вершин графа установлен на последнюю вершину"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 3:{
					++*vIt;
			        cout<<"        Итератор вершин графа установлен на следующую позицию"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 4:{
					string cur;
					try{
						cur=vIt->read_vertex();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Вершина на текущей позиции итератора: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 5:{
					int cur;
					try{
						cur=vIt->read_data_vertex();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Данные вершины на текущей позиции итератора: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 6:{
					int cur;
					cout<<"        Введите данные: ";
					cin>>cur;
					try{
						vIt->write_data_vertex(cur);
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Данные вершины записаны по текущей позиции итератора"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 7:{
					string cur;
					cout<<"        Введите имя: ";
					cin>>cur;
					try{
						vIt->write_name_vertex(cur);
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Имя вершины записано по текущей позиции итератора"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case -1:{
					flag=true;
					delete vIt;
					cout<<"===================================="<<endl;
						};
					break;
				default: {
					cout<<"        Неверно выбран пункт меню!"<<endl;
					cout<<"        ===================================="<<endl;
					cout<<"        Введите верный пункт: "; 
						 };
					break;
				}
			}
				};
			break;
		case 14:{
			bool flag=false;
			eIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator(*graph);
			cout<<"Итератор рёбер: "<<endl;
			cout<<"        1 - Установить итератор рёбер на начальное ребро"<<endl;
			cout<<"        2 - Установить итератор рёбер на конечное ребро"<<endl;
			cout<<"        3 - Переход к следующей позиции"<<endl;
			cout<<"        4 - Прочитать исходящую, входящую вершины ребра и вес"<<endl<<"        по текущей позиции итератора"<<endl;
			cout<<"        5 - Прочитать данные ребра по текущей позиции итератора"<<endl;
			cout<<"        6 - Записать данные о ребре по текущей позиции итератора"<<endl;
			cout<<"        7 - Записать вес ребра по текущей позиции итератора"<<endl;
			cout<<"        -1 -Выход из меню итератора"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
				case 1:{
					bool rez;
					rez=eIt->begin();
					if (rez)
						cout<<"        Итератор рёбер графа установлен на первое ребро"<<endl;
					else
						cout<<"        False. Рёбер в графе нет"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 2:{
					bool rez;
			        rez=eIt->toend();
					if(rez)
						cout<<"        Итератор рёбер графа установлен на последнее ребро"<<endl;
					else
						cout<<"        False. Рёбер в графе нет"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 3:{
					bool rez;
					rez=++*eIt;
					if(rez)
						cout<<"        Итератор рёбер графа установлен на следующую позицию"<<endl;
					else
						cout<<"        Null"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 4:{
					string cur;
				    try{
						cur=eIt->read_edge();
					}
					catch(char* Error){
						cout<<"        Null"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					};
			        cout<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 5:{
					int cur;
					try{
						cur=eIt->read_data_edge();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Данные ребра на текущей позиции итератора: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 6:{
					int cur;
					bool rez;
					cout<<"        Введите данные: ";
					cin>>cur;
					rez=eIt->write_data_edge(cur);
					if (!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Данные ребра записаны по текущей позиции итератора"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 7:{
				    int w;
					bool rez;
					cout<<"        Введите вес: ";
					cin>>w;
					rez=eIt->write_weight_edge(w);
					if(!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Вес ребра записан по текущей позиции итератора"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case -1:{
					flag=true;
					delete eIt;
					cout<<"===================================="<<endl;
						};
					break;
				default: {
					cout<<"        Неверно выбран пункт меню!"<<endl;
					cout<<"        ===================================="<<endl;
					cout<<"        Введите верный пункт: "; 
						 };
					break;
				}
			}
			};
			break;
        case 15:{
			bool flag=false;
			int v;
			cout<<"Введите вершину: ";
			cin>>v;
			oIt= new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::OutputEdgeIterator(*graph, *graph->getVertex(v));
			cout<<"Итератор исходящих рёбер: "<<endl;
			cout<<"        1 - Установить итератор исходящих рёбер на начальное ребро"<<endl;
			cout<<"        2 - Установить итератор исходящих рёбер на конечное ребро"<<endl;
			cout<<"        3 - Переход к следующей позиции"<<endl;
			cout<<"        4 - Прочитать исходящую, входящую вершины ребра и вес"<<endl<<"        по текущей позиции итератора"<<endl;
			cout<<"        5 - Прочитать данные ребра по текущей позиции итератора"<<endl;
			cout<<"        6 - Записать данные о ребре по текущей позиции итератора"<<endl;
			cout<<"        7 - Записать вес ребра по текущей позиции итератора"<<endl;
			cout<<"        -1 -Выход из меню итератора"<<endl;
			cout<<"        >>";
			while (!flag){
				menu=enter_menu();
				switch (menu){
				case 1:{
					bool rez;
					rez=oIt->begin();
					if (rez)
						cout<<"        Итератор исходящих рёбер графа установлен на первое ребро"<<endl;
					else
						cout<<"        False. Рёбер в графе нет"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 2:{
					bool rez;
			        rez=oIt->toend();
					if(rez)
						cout<<"        Итератор исходящих рёбер графа установлен на последнее ребро"<<endl;
					else
						cout<<"        False. Рёбер в графе нет"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 3:{
					bool rez;
					rez=++*oIt;
					if(rez)
						cout<<"        Итератор исходящих рёбер графа установлен на следующую позицию"<<endl;
					else
						cout<<"        Null"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 4:{
					string cur;
				    try{
						cur=oIt->read_edge();
					}
					catch(char* Error){
						cout<<"        Null"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					};
			        cout<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 5:{
					int cur;
					try{
						cur=oIt->read_data_edge();
					}
					catch (char* Error){
						cout<<Error<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Данные ребра на текущей позиции итератора: "<<cur<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 6:{
					int cur;
					bool rez;
					cout<<"        Введите данные: ";
					cin>>cur;
					rez=oIt->write_data_edge(cur);
					if (!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Данные ребра записаны по текущей позиции итератора"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case 7:{
				    int w;
					bool rez;
					cout<<"        Введите вес: ";
					cin>>w;
					rez=oIt->write_weight_edge(w);
					if(!rez){
						cout<<"        False"<<endl;
						cout<<"        ===================================="<<endl;
			            cout<<"        Введите пункт меню: ";
						break;
					}
			        cout<<"        Вес ребра записан по текущей позиции итератора"<<endl;
			        cout<<"        ===================================="<<endl;
			        cout<<"        Введите пункт меню: ";
					   };
					break;
				case -1:{
					delete oIt;
					flag=true;
					cout<<"===================================="<<endl;
						};
					break;
				default: {
					cout<<"        Неверно выбран пункт меню!"<<endl;
					cout<<"        ===================================="<<endl;
					cout<<"        Введите верный пункт: "; 
						 };
					break;
				}
			}
			};
			break;
            case 16:{
				bool flag=false;
				cout<<"Операции над ребром:"<<endl;
				cout<<"        1 - Прочитать вес ребра"<<endl;
				cout<<"        2 - Прочитать данные о ребре"<<endl;
				cout<<"        3 - Записать данные о ребре"<<endl;
				cout<<"        4 - Записать вес ребра"<<endl;
				cout<<"        -1 -Выход из меню"<<endl;
				cout<<"        >>";
				while (!flag){
					menu=enter_menu();
					switch (menu){
					case 1:{
						string v1, v2;
						int w;
						cout<<"        Введите имя исходящей вершины: ";
						cin>>v1;
						cout<<"        Введите имя входящей вершины: ";
						cin>>v2;
						try{
							w=graph->read_weight_edge(v1, v2);
						}
						catch(char* Error){
							cout<<Error<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        Введите пункт меню: ";
							break;
						};
						cout<<"        Вес ребра: "<<w<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите пункт меню: ";
						   };
						break;
					case 2:{
						int d;
						string v1, v2;
						cout<<"        Введите имя исходящей вершины: ";
						cin>>v1;
						cout<<"        Введите имя входящей вершины: ";
						cin>>v2;
						try{
							d=graph->read_data_edge(v1, v2);
						}
						catch (char* Error){
							cout<<Error<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        Введите пункт меню: ";
							break;
						}
						cout<<"        Данные ребра: "<<d<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите пункт меню: ";
						   };
						break;
					case 3:{
						int d;
						string v1, v2;
						bool rez;
						cout<<"        Введите имя исходящей вершины: ";
						cin>>v1;
						cout<<"        Введите имя входящей вершины: ";
						cin>>v2;
						cout<<"        Введите данные: ";
						cin>>d;
						rez=graph->write_data_edge(v1, v2, d);
						if (!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        Введите пункт меню: ";
							break;
						}
						cout<<"        Данные о вершине записаны"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите пункт меню: ";
						   };
						break;
					case 4:{
						bool rez;
						string v1, v2;
						int w;
						cout<<"        Введите имя исходящей вершины: ";
						cin>>v1;
						cout<<"        Введите имя входящей вершины: ";
						cin>>v2;
						cout<<"        Введите вес: ";
						cin>>w;
						rez=graph->write_weight_edge(v1, v2, w);
						if(!rez){
							cout<<"        False"<<endl;
							cout<<"        ===================================="<<endl;
							cout<<"        Введите пункт меню: ";
							break;
						}
						cout<<"        Вес ребра перезаписан"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите пункт меню: ";
						   };
						break;
					case -1:{
						flag=true;
						cout<<"===================================="<<endl;
							};
						break;
					default: {
						cout<<"        Неверно выбран пункт меню!"<<endl;
						cout<<"        ===================================="<<endl;
						cout<<"        Введите верный пункт: "; 
							 };
						break; 
				};
			};
				}
			break;
		case 20:{
			Tt1 = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::Task1(*graph);
			bool temp = false;
			cout << "Введите вершну: " << endl;
			int v;
			cin >> v;
			cout << "Введите длину цикла: " << endl;
			int cic;
			cin >> cic;

			if (temp) {
				cout << endl << "Цикл: " << endl;
			}
			else cout << "Нет цикла" << endl;
			temp = Tt1->restart(v, cic);

			cout<<endl<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
				};
			break;
		case 21:{
			Task2<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>* taskTwo;
			taskTwo = new Task2<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(graph);
			
			cout<<"Результат:" << endl << taskTwo->result()<<endl;
			cout<<"Задача решена"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите пункт меню: ";
				};
			break;
		case -1: {
			return;
				 };			break;
		default: {
			cout<<"Неверно выбран пункт меню!"<<endl;
			cout<<"===================================="<<endl;
			cout<<"Введите верный пункт: ";
				 };
			break;
		}
	}

	getch();
	return;
}