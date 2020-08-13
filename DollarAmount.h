/*
 *  Name: DollarAmount.h
 *
 *  Desc: This class allows the user to perform integer arithmetic on dollar
 *        values threrby avoiding rounding and percision issues. 
 */
//#ifndef DOLLARAMOUNT_H
//#define DOLLARAMOUNT_H
#include <string>
#include <cmath>

class DollarAmount {
public:
  //initialize amount from an int64_t value
  explicit DollarAmount(int64_t value) : amount{value} {}

  void add(DollarAmount right) {
    amount += right.amount;
  }
  
  void subtract(DollarAmount right) {
    amount -= right.amount;
  }
  
  void addInterest(int rate, int divisor) {
    DollarAmount interest {(amount * rate + divisor / 2) / divisor};
    add(interest);
}
    
  std::string toString() const {
    std::string dollars{std::to_string(amount / 100)};
    std::string cents{std::to_string(std::abs(amount % 100))};
    return dollars + "." + (cents.size() == 1 ? "0" : "") + cents; 
}
    
private:
  int64_t amount{0};  // dollar amount in pennies
};

      
