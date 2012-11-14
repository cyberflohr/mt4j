//+------------------------------------------------------------------+
//|                                                    mt4Sample.mq4 |
//|                                       Copyright © 2011, Flohr IT |
//|                                            http://www.flohrit.de |
//+------------------------------------------------------------------+
#property copyright "Copyright © 2011, Flohr-IT"
#property link      "http://www.flohrit.de"

#include <mt4j.mqh>
#include <stdlib.mqh>

int jCtx;
int tickCounter;

extern int IntProperty = 99;
extern bool BoolProperty = True;
extern double DoubleProperty = 1.23;
extern string StringProperty = "helloWorld";

//+------------------------------------------------------------------+
//| expert initialization function                                   |
//+------------------------------------------------------------------+
int init()
  {
//----
   jCtx = newInstance("de/flohrit/mt4j/SampleEA");
   setIntProperty(jCtx, "IntPropertySample", IntProperty);
   if (IntProperty != getIntProperty(jCtx, "IntPropertySample")) {
      Alert("Int wasn't set correct: " + getIntProperty(jCtx, "IntPropertySample"));
   }

   setDoubleProperty(jCtx, "DoublePropertySample", DoubleProperty);
   if (DoubleProperty != getDoubleProperty(jCtx, "DoublePropertySample")) {
      Alert("Double wasn't set correct: " + getDoubleProperty(jCtx, "DoublePropertySample"));
   }

   setStringProperty(jCtx, "StringPropertySample", StringProperty);
   if (StringProperty != getStringProperty(jCtx, "StringPropertySample")) {
      Alert("String wasn't set correct: " + getStringProperty(jCtx, "StringPropertySample"));
   }

   setBoolProperty(jCtx, "BoolPropertySample", BoolProperty);
   if (BoolProperty != getBoolProperty(jCtx, "BoolPropertySample")) {
      Alert("Boolean wasn't set correct: " + getBoolProperty(jCtx, "BoolPropertySample"));
   }

   // calls a noargs void method in your EA class.
   callNoArgsMethod(jCtx, "doSomething");
   
   updateBarInfo(jCtx);
   initObject(jCtx);
}

//+------------------------------------------------------------------+
//| expert deinitialization function                                 |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   deinitObject(jCtx);
   destroy(jCtx);   
   Alert("tickCounter: " + tickCounter);
//----
   return(0);
  }
  
//+------------------------------------------------------------------+
//| expert start function                                            |
//+------------------------------------------------------------------+
int start()
  {
//----
   tickCounter++;
   updateBarInfo(jCtx);
   
   int processAction = processTick(jCtx, Bid, Ask);   
   switch (processAction) {
      case PROCESS_TICK_NONE:
      break;

      case PROCESS_TICK_DO_BUY:
      Alert("Execute buy order...");
      break;

      case PROCESS_TICK_DO_SELL:
      Alert("Execute sell order...");
      break;
   }
   
   return(0);
  }
  

//+------------------------------------------------------------------+