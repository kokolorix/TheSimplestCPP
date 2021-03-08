/**
 * @brief Comparable objects
 * @details
 * usually it is sufficient to define the less operator,
 * to get completely comparable types.
 */
struct Comparable
{
   virtual bool operator < (const Comparable& other) const
   {
      return this < &other;
   }

   virtual bool operator==(const Comparable& other) const
   {
      return !(*this < other) && !(other < *this);
   }

   virtual bool operator!=(const Comparable& other) const
   {
      return (*this < other || other < *this);
   }

   virtual bool operator<=(const Comparable& other) const
   {
      return *this < other || !(other < *this);
   }

   virtual bool operator>(const Comparable& other) const
   {
      return other < *this;
   }

   virtual bool operator>=(const Comparable& other) const
   {
      return other < *this || !(*this < other);
   }
};
