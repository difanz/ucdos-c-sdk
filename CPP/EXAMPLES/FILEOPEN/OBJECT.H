/**  *************  +/
      UC AObject
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_AOBJECT_CLASS
#define SDK_AOBJECT_CLASS

class AObject
  {
    public:
      AObject() { };
      virtual ~AObject() { };

      virtual void OnChange( UINT Type, UINT IDcom) = 0;
  };

#endif
