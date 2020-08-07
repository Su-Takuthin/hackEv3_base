#include "VirtualLineTracer.h"
#include "math.h"


VirtualLineTracer::VirtualLineTracer(Odometry *odo,
                        SpeedControl *scon)
                         : SimpleWalker(odo,scon) {
    cx=0;
    cy=0;
    ax=0;
    ay=0;
    basedistance=0;
    currentdistance=0;
    mTarget=0;
    mTargetSpeed=0;
    mPFactor=0;
    mIFactor=0;
    mDFactor=0;
}

void VirtualLineTracer::setParam(float speed,float kp, float ki, float kd,float angleTarget,float angleKp){

    mTargetSpeed = speed;
    
    mPFactor = kp;
    mIFactor = ki;
    mDFactor = kd;

    setBaseDistance();
    mTarget= basedistance;
    mPid->setTarget(mTarget);


    
    mPid->setKp(mPFactor); 
    mPid->setKi(mIFactor);
    mPid->setKd(mDFactor);
   
    mCurve = angleTarget;
    mAngleKp = angleKp;


    currentdistance = calcDistance();
}

void VirtualLineTracer::setCenterPosition(float centerx,float centery){
    cx=centerx;
    cy=centery;
}

void VirtualLineTracer::setBaseDistance(){
    

    ax=mXPosition->getvalue();
    ay=mYPosition->getvalue();

    basedistance = calcDistance();
}

float VirtualLineTracer::calcDistance(){
    return  sqrt((ax+7*cos(mTurnAngle->getValue())-cx)*(ax+7*cos(mTurnAngle->getValue())-cx)+(ay+7*sin(mTurnAngle->getValue())-cy)*(ay+7*sin(mTurnAngle->getValue())-cy));
}

float VirtualLineTracer::calcTurn(){


        float val1_turn =  mPid->getOperation(basedistance);
        return val1_turn ;
}

void VirtualLineTracer::run(){
    

    setBaseDistance();
    mTurn = calcTurn();
    static char buf[100];
    sprintf(buf,"%f",mTurn);
    msg_log(buf);

    setCommandV((int)mTargetSpeed, (int)mTurn);
    SimpleWalker::run();
}

 