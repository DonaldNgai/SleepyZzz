#define winSize 250
#define M 10
#define N 30
#define HP_CONSTANT ((float) 1 / (float) M)
#define RAND_RES 100000000
#define HB_DIFF_RANGE 40
#define HB_STANDARD_BPM 75

unsigned int last_beat_time;

/* Portion pertaining to Pan-Tompkins QRS detection */

// circular buffer for input ecg signal
// we need to keep a history of M + 1 samples for HP filter
float ecg_buff[M + 1] = {0};
int ecg_buff_WR_idx = 0;
int ecg_buff_RD_idx = 0;

// circular buffer for input ecg signal
// we need to keep a history of N+1 samples for LP filter
float hp_buff[N + 1] = {0};
int hp_buff_WR_idx = 0;
int hp_buff_RD_idx = 0;

// LP filter outputs a single point for every input point
// This goes straight to adaptive filtering for eval
float next_eval_pt = 0;

// running sums for HP and LP filters, values shifted in FILO
float hp_sum = 0;
float lp_sum = 0;

// working variables for adaptive thresholding
float threshold = 0;
boolean triggered = false;
int trig_time = 0;
float win_max = 0;
int win_idx = 0;

// numebr of starting iterations, used determine when moving windows are filled
int number_iter = 0;

int tmp = 0;

unsigned int current_beat_time;
int last_hr, current_hr;

void get_heartrate(){
  #if defined DEBUG && defined SHOW_HEART
    Serial.print(analogRead(heartPin));
  #endif
    if(detect(analogRead(heartPin))){
      #if defined DEBUG && defined SHOW_HEART
        Serial.print(",");
        Serial.println(1000);
      #endif
      current_beat_time = millis();
      current_hr = (int) (60.0 / ((current_beat_time - last_beat_time)/1000.0));
     
      if((current_hr < last_hr + HB_DIFF_RANGE) || (current_hr < (HB_STANDARD_BPM + HB_DIFF_RANGE))){
//        Serial.println(current_beat_time - last_beat_time);
//        Serial.print("                           ");
//        Serial.println(current_hr);
        if(current_hr < last_hr*0.7){
          current_hr = last_hr*0.8;
        }
//        Serial.println(current_hr);
//        Serial.println();
        HeartRateAverage.add_value(current_hr,&averagedHeartRate);

        if((last_hr == 87) && (current_hr == 87)){
          number_iter = 0;
          win_idx = 0;
          win_max = -10000000;
          hp_sum = 0;
          lp_sum = 0;
        }
        last_hr = current_hr;
        last_beat_time = current_beat_time;
        
      } 
    } else {
      delay(1);
      #if defined DEBUG && defined SHOW_HEART
        Serial.print(",");
        Serial.println(0);
      #endif
      }
}

bool detect(float new_ecg_pt) {
  // copy new point into circular buffer, increment index
  ecg_buff[ecg_buff_WR_idx++] = new_ecg_pt;

  ecg_buff_WR_idx %= (M+1);
 
 
  /* High pass filtering */
  if(number_iter < M){
    // first fill buffer with enough points for HP filter
    hp_sum += ecg_buff[ecg_buff_RD_idx];
    hp_buff[hp_buff_WR_idx] = 0;
  }
  else{
    hp_sum += ecg_buff[ecg_buff_RD_idx];
    
    tmp = ecg_buff_RD_idx - M;
    if(tmp < 0) tmp += M + 1;
    
    hp_sum -= ecg_buff[tmp];
    
    float y1 = 0;
    float y2 = 0;
    
    tmp = (ecg_buff_RD_idx - ((M+1)/2));
    if(tmp < 0) tmp += M + 1;
    
    y2 = ecg_buff[tmp];
    
    y1 = HP_CONSTANT * hp_sum;
    
    hp_buff[hp_buff_WR_idx] = y2 - y1;
  }
  
  // done reading ECG buffer, increment position
  ecg_buff_RD_idx++;
  ecg_buff_RD_idx %= (M+1);
  
  // done writing to HP buffer, increment position
  hp_buff_WR_idx++;
  hp_buff_WR_idx %= (N+1);
  

  /* Low pass filtering */
  
  // shift in new sample from high pass filter
  lp_sum += hp_buff[hp_buff_RD_idx] * hp_buff[hp_buff_RD_idx];
  
  if(number_iter < N){
    // first fill buffer with enough points for LP filter
    next_eval_pt = 0;
    
  }
  else{
    // shift out oldest data point
    tmp = hp_buff_RD_idx - N;
    if(tmp < 0) tmp += (N+1);
    
    lp_sum -= hp_buff[tmp] * hp_buff[tmp];
    
    next_eval_pt = lp_sum;
  }
  
  // done reading HP buffer, increment position
  hp_buff_RD_idx++;
  hp_buff_RD_idx %= (N+1);
  

  /* Adapative thresholding beat detection */
  // set initial threshold        
  if(number_iter < winSize) {
    if(next_eval_pt > threshold) {
      threshold = next_eval_pt;
    }

    // only increment number_iter iff it is less than winSize
    // if it is bigger, then the counter serves no further purpose
    number_iter++;
  }
  
  // check if detection hold off period has passed
  if(triggered == true){
    trig_time++;
    
    if(trig_time >= 100){
      triggered = false;
      trig_time = 0;
    }
  }
  
  // find if we have a new max
  if(next_eval_pt > win_max) win_max = next_eval_pt;
  
  // find if we are above adaptive threshold
  if(next_eval_pt > threshold && !triggered) {
    triggered = true;

    return true;
  }
        // else we'll finish the function before returning FALSE,
        // to potentially change threshold
          
  // adjust adaptive threshold using max of signal found 
  // in previous window            
  if(win_idx++ >= winSize){
    // weighting factor for determining the contribution of
    // the current peak value to the threshold adjustment
    float gamma = 0.175;
    
    // forgetting factor - 
    // rate at which we forget old observations
                // choose a random value between 0.01 and 0.1 for this, 
    float alpha = 0.01 + ( ((float) random(0, RAND_RES) / (float) (RAND_RES)) * ((0.1 - 0.01)));
    
                // compute new threshold
    threshold = alpha * gamma * win_max + (1 - alpha) * threshold;
    
    // reset current window index
    win_idx = 0;
    win_max = -10000000;
  }
      
        // return false if we didn't detect a new QRS
  return false;
    
}

