#include<iostream>
#include<algorithm>

using namespace std;
class Item {
  int itemID;
  int price;
  int weight;
  float ratio;
  bool selected;

  public:
  Item(int itemID,int price,int weight) {
    this->price = price;
    this->weight = weight;
    this->ratio = ((float)price)/weight;
    this->itemID = itemID;
  }

  int getPrice() {
    return price;
  }

  int getWeight() {
    return weight;
  }
  int getItemId() {
    return itemID;
  }
  bool isSelected() {
    return selected;
  }
  void setSelected(bool selected) {
    this->selected = selected;
  }
  float getRatio() {
    return ratio;
  }
};

bool comp(Item* i1,Item* i2) {
  return (i1->getRatio())>(i2->getRatio());
}

void solve_knapsack_01(int cap,Item** item_array,int n) {
  sort(item_array,item_array+n,comp);
  int capacity = cap;
  for(int i=0;i<n;i++) {
    if(capacity >= item_array[i]->getWeight()) {
      capacity = capacity-item_array[i]->getWeight();
      item_array[i]->setSelected(true);
    }
    else {
      item_array[i]->setSelected(false);
    }
  }
}

int main() {
  int n,cap;
  cout<<"Enter the number of items\n";
  cin>>n;
  Item** item_array = new Item*[n];
  int price,weight;
  for(int i=0;i<n;i++) {
    cout<<"Enter the price for item "<<i<<" : ";
    cin>>price;
    cout<<"Enter the weight for item "<<i<<" : ";
    cin>>weight;
    item_array[i] = new Item(i,price,weight);
  }
  cout<<"Enter the bag capacity\n";
  cin>>cap;
  solve_knapsack_01(cap,item_array,n);

  for(int i=0;i<n;i++) {
    cout<<"Item "<<item_array[i]->getItemId()<<" selected : "<<item_array[i]->isSelected()<<endl;
  }
  cout<<endl;
}
