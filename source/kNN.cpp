#include "kNN.hpp"


//---------BEGIN STRUCT NODE-----------//
template<typename T>
Node<T>::Node(T data, Node<T>* next, Node<T>* prev){
    this->data = data;
    this->next = next;
    this->prev = prev;
}

template<typename T>
Node<T>::Node(){
    this->data = 0;
    this->next = NULL;
    this->prev = NULL;
}

//---------END STRUCT NODE-----------//



//---------BEGIN CLASS LIST-----------//
template <typename T>
MyList<T>::MyList(){
    this->head = NULL;
    this->tail = NULL;
    this->count = 0;
}

template <typename T>
MyList<T>::~MyList(){
    clear();
}

template <typename T>
void MyList<T>::push_back(T value){
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}

template <typename T>
void MyList<T>::push_front(T value){
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    count++;
}



template<typename T>
void MyList<T>::insert(int index, T value){
    if (index < 0 || index > count) {
        return;
    } else {
        if (index == 0) {
            push_front(value);
        } else if (index == count) {
            push_back(value);
        } else {
            Node<T>* newNode = new Node<T>(value);
            Node<T>* current = this->head;
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
            Node<T>* nextNode = current->next;
            newNode->prev = current;
            newNode->next = nextNode;
            current->next = newNode;
            nextNode->prev = newNode;
            count++;
        }
    }
}


template<typename T>
void MyList<T>::remove(int index) {
    if (index < 0 || index >= count) {
        return;
    }
    if (index == 0) {
        Node<T>* nodeToRemove = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr; 
        }
        delete nodeToRemove;
    } else if (index == count - 1) {
        Node<T>* nodeToRemove = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        } else {
            head = nullptr; 
        }
        delete nodeToRemove;
    } else {
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        Node<T>* nodeToRemove = current;
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete nodeToRemove;
    }
    count--;
}

template<typename T>
T& MyList<T>::get(int index) const {
    if (index < 0 || index >= this->count) {
        throw std::out_of_range("get(): Out of range");
    }else {
        Node<T>* getNode;
        if (index < this->count/2){
            getNode = this->head;
            for (int i = 0; i < index; i++) {
                getNode = getNode->next;
            }
        }else{
            getNode = this->tail;
            for (int i = this->count - 1; i > index; i--){
                getNode = getNode->prev;
            }
        }
        return getNode->data;
    }
}

template<typename T>
void MyList<T>::set(int index, T value) {
    if (index < 0 || index >= this->count) {
        throw std::out_of_range("set(): Out of range");
    } else {
        Node<T>* tmp;
        if(index < this->count / 2){
            tmp = this->head;
            for (int i = 0; i < index; i++){
                tmp = tmp->next;
            }
        }else{
            tmp = this->tail;
            for (int i = this->count - 1; i > index; i--){
                tmp = tmp->prev;
            }
        }
        tmp->data = value;
    }
}


template<typename T>
int MyList<T>::length() const{
    return this->count;
}

template<typename T>
void MyList<T>::clear(){
    while(this->head != NULL){
        Node<T>* temp = this->head;
        this->head = this->head->next;
        delete temp;
    }
    this->tail = NULL;
    this->count = 0;
}

template<typename T>
void MyList<T>::print() const {
    Node<T>* temp = this->head;
    while(temp != NULL){
        cout << temp->data;
        if(temp->next != NULL){
            cout << " ";
        }
        temp = temp->next;
    }
    delete[] temp;
}

template<typename T>
void MyList<T>::reverse() {
    if (head == nullptr || head->next == nullptr) {
        return;
    }
    Node<T>* current = head;
    Node<T>* previous = nullptr;
    Node<T>* next = nullptr;
    while (current != nullptr) {
        next = current->next;  
        current->next = previous;  
        current->prev = next;  
        previous = current;  
        current = next;  
    }
    tail = head;
    head = previous;
}



template<typename T>
bool MyList<T>::isEmpty() const {
    return (count == 0);
}


template<typename T>
int MyList<T>::indexOf(T name){
    Node<T>* temp = this->head;
    for(int i = 0; i < this->count; i++){
        if(temp->data == name){
            return i;
        }
        temp = temp->next;
    }
    return -1;
}

//---------END CLASS LIST-----------//




//---------BEGIN CLASS DATASET-----------//



Dataset::Dataset(){
    this->data = new MyList<MyList<int>*>();
    this->columnNames = new MyList<string>();
}

Dataset::~Dataset(){
        // delete data;
        // delete columnNames;
    if(this->data){
        delete data;
    }
    if(this->columnNames){
        delete this->columnNames;
    }
}

Dataset::Dataset(const Dataset& other){
    data = new MyList<MyList<int>*>(*(other.data));
    columnNames = new MyList<string>(*(other.columnNames));
}

Dataset& Dataset::operator=(const Dataset& other){
    if(this == &other){
        return *this;
    }
        delete this->columnNames;
    this->columnNames = new MyList<string>();
    for(int i = 0; i < other.getNameLables()->length(); i++){
        columnNames->push_back(other.getNameLables()->get(i));
    }
        delete this->data;
    this->data = new MyList<MyList<int>*>();
    int nRows = other.getData()->length();
    int nCols = other.getData()->get(0)->length();
    for(int i = 0; i < nRows; i++){
        MyList<int>* tmp = new MyList<int>();
        for(int j = 0; j < nCols; j++){
            tmp->push_back(other.getData()->get(i)->get(j));
        }
        this->data->push_back(tmp);
    }
    return *this;

}


bool Dataset::loadFromCSV(const char* fileName){
    fstream input_file(fileName, ios::in);
    if(!input_file.is_open()){
        return false;
    }else{
        this->data->clear();
        this->columnNames->clear();
        string line;
        getline(input_file, line);
        stringstream ss(line);
        string token;
        while(getline(ss, token, ',')){
            columnNames->push_back(token);
        }
        while(getline(input_file, line)){
            stringstream ss(line);
            MyList<int>* tmp = new MyList<int>();
            int value;
            while(ss >> value){
                tmp->push_back(value);
                if(ss.peek() == ','){
                    ss.ignore();
                }
            }
            this->data->push_back(tmp);
        }
        input_file.close();
        return true;
    }
}

void Dataset::printHead(int nRows, int nCols) const{
    // exception handling
    if(this->getData()->isEmpty() && this->getNameLables()->isEmpty()) return;
    if(nRows < 0 || nCols < 0) return;
    if(nRows > this->data->length()){
        nRows = this->data->length();
    }
    if(nCols > this->columnNames->length()){
        nCols = this->columnNames->length();
    }

    // get column names
    for (int i = 0; i < nCols ; i++) {
        cout << columnNames->get(i);
        if(i != nCols - 1) cout<<" ";
    }
    cout << endl;
    // get data
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            cout << this->data->get(i)->get(j);
            if(j != nCols-1){
                cout << " ";
            }
        }
        if(i != nRows-1){
            cout << endl;
        }
    }

}

void Dataset::printTail(int nRows, int nCols) const{
    // handle exception
    if (nRows <= 0 || nCols <= 0) {
        return;
    }
    int lenghtOfCol = data->get(0)->length();
    if (nCols > lenghtOfCol) {
        nCols = lenghtOfCol;
    }

    if (nRows > data->length()) {
        nRows = data->length();
    }
    // 
    int n = this->data->length();
    int m = this->data->get(0)->length();
    for(int i = m-nCols; i <= m-1; i++){
        cout << this->columnNames->get(i);
        if(i != m-1) cout << " ";

    }
    cout << endl;
    for(int i = n-nRows; i <= n-1; i++){
        for(int j = m-nCols; j <= m-1; j++){
            cout << this->data->get(i)->get(j);
            if(j != m-1) cout << " ";
        }
        if(i != n-1){
            cout << endl;
        }
    }

}


void Dataset::getShape(int& nRows, int& nCols) const{
    nRows = this->data->length();
    nCols = this->columnNames->length();
}



void Dataset::columns() const{
    for (int i = 0; i < columnNames->length() - 1; i++) {
        cout << columnNames->get(i) << " ";
    }
    cout << columnNames->get(columnNames->length() - 1);
}

bool Dataset::drop(int axis, int index, string columnName){
    if(axis != 0 && axis != 1){
        return false;
    }
    else{
        if(axis == 0){
            if(index < 0 || index >= this->data->length()){
                return false;
            }
            this->data->remove(index);
            return true;
            
        }else{
            int index = this->columnNames->indexOf(columnName);
            if(index == -1){
                return false;
            }else{
                this->columnNames->remove(index);
                for(int i = 0; i < data->length(); i++){
                    data->get(i)->remove(index);
                }
                return true;
            }
        }
    }
}

Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const{

    Dataset res;
    if(endRow == -1 || endRow >= this->data->length()){
        endRow = this->data->length()-1;
    }
    if(endCol == -1 || endCol >= this->columnNames->length()){
        endCol = this->columnNames->length()-1;
    }

    for(int i = startCol; i <= endCol; i++){
        res.columnNames->push_back(this->columnNames->get(i));
    }

    for(int i = startRow; i <= endRow; i++){
        MyList<int>* tmp = new MyList<int>();
        for(int j = startCol; j <= endCol; j++){
            tmp->push_back(this->data->get(i)->get(j));
        }
        res.data->push_back(tmp);
    }

    return res;
}

MyList<MyList<int>*>* Dataset::getData() const{
    return this->data;
}

MyList<string>* Dataset::getNameLables() const{
    return this->columnNames;
}

int Dataset::getLenghtData() const{
    return this->data->length();
}

int Dataset::getLenghtNameLables() const{
    return this->columnNames->length();
}


// ---------------- Class kNN ---------------- //
kNN::kNN(int k){
    this->k = k;
}

void kNN::fit(const Dataset& X_train, const Dataset& y_train){
    this->X_train = X_train;
    this->Y_train = y_train;
}

double minValue(MyList<double>*list, int start){
    if(start < 0) {
        throw std::out_of_range("get(): Out of range");
        return -1;
    }
    double min = list->get(start);
    for (int i = start + 1; i < list->length(); i++) {
        if (list->get(i) < min) {
            if (list->get(i) >= 0){
                min = list->get(i);
            }
        }
    }
    return min;
}

int indexOfMinValue(MyList<double>*list, double value, int start){
    if(start < 0) {
    throw std::out_of_range("get(): Out of range");
    return -1;
}
    for (int i = start; i < list->length(); i++) {
    if (list->get(i) == value) {
         return i;
        }
    }
    return -1;
}


double kNN::euclidean_distance(MyList<int>* p1, MyList<int>* p2) const{
    if (p1->length() != p2->length()) {
        throw std::invalid_argument("Don't match");
    }

    double sum = 0.0;
    for (int i = 0; i < p1->length(); i++) {
        double diff = p1->get(i) - p2->get(i);
        sum += diff * diff;
    }
    return sqrt(sum);
}



int kNN::findKNN(MyList<double>* distances, int k) const{
    MyList<double>* tmp = new MyList<double>();
    MyList<int>* index = new MyList<int>();
    for(int i = 0 ; i < distances->length(); i++){
        tmp->push_back(distances->get(i));
        index->push_back(i);
    }

    // Sorting
    if(distances->length() > k){
        for(int i = 0; i < k; i++){
            double min_value = minValue(tmp, i);
            int min_index = indexOfMinValue(tmp, min_value, i);

            if(min_index != i){
                std::swap(tmp->get(i), tmp->get(min_index));
                std::swap(index->get(i), index->get(min_index));
            }
        }
    }
    

    int res_value = this->Y_train.getData()->get(index->get(0))->get(0);
    int res_cnt = 1;
    int n = std::min(k, tmp->length()); 

    for (int i = 0; i < n; i++) {
        int current_value = this->Y_train.getData()->get(index->get(i))->get(0);
        int current_cnt = 1; 

        for (int j = i + 1; j < n; j++) {
            if (this->Y_train.getData()->get(index->get(j))->get(0) == current_value) {
                current_cnt++;
            }
        }

        if (current_cnt > res_cnt || (current_cnt == res_cnt && current_value < res_value)) {
            res_cnt = current_cnt;
            res_value = current_value;
        }
    }

    delete tmp;
    delete index;
    return res_value;
}


Dataset kNN::predict(const Dataset& X_test){
    MyList<MyList<int>*>* data = X_test.getData();
    Dataset res;
    if(this->X_train.getNameLables()->length() != 0)
        res.getNameLables()->push_back("label");
    int n_X_test = data->length();
    for(int i = 0; i < n_X_test; i++){
        MyList<double>* distances = new MyList<double>();
        int n_lenght = this->X_train.getLenghtData();
        for(int j = 0; j < n_lenght; j++){
            distances->push_back(euclidean_distance(data->get(i), this->X_train.getData()->get(j)));
        }
        int lable_y_train = findKNN(distances, this->k);
        MyList<int>* tem = new MyList<int>();
        tem->push_back(lable_y_train);
        res.getData()->push_back(tem);
        delete distances;
    }
    return res;
}




double kNN::score(const Dataset& y_test, const Dataset& y_pred) {
    const MyList<MyList<int>*>& y_test_data = *y_test.getData();
    const MyList<MyList<int>*>& y_pred_data = *y_pred.getData();

    int n = y_test_data.length();
    if (n == 0) {
        return -1;
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (y_test_data.get(i)->get(0) == y_pred_data.get(i)->get(0)) {
            count++;
        }
    }

    return static_cast<double>(count) / static_cast<double>(n);
}



void train_test_split(Dataset& X, Dataset& y, double test_size, 
                        Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test){
    int temp = X.getLenghtData();
    if(test_size == 1) return;

    int nX= (1 - test_size)*temp;
    X_train = X.extract(0, nX - 1, 0, -1);
    y_train = y.extract(0, nX - 1, 0, 0);
    X_test = X.extract(nX, -1, 0, -1);
    y_test = y.extract(nX, -1, 0, 0);
}



