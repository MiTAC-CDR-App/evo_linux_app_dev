#ifndef LOGMACRO_H_
#define LOGMACRO_H_

#include <stdio.h>

#define LOG(format, args...) printf(format "\n", ##args)
#define LOGI(format, args...) LOG("[INFO] " format, ##args)
#define LOGW(format, args...) LOG("[WARN] " format, ##args)
#define LOGE(format, args...) LOG("[ERROR] " format, ##args)

#endif /* LOGMACRO_H_ */