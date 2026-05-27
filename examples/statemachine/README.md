# 状态机 APP 简介

## **状态机功能介绍**

状态机主要由以下几个部分组成：

状态集合：一组有限的状态，每个状态代表系统的一种特定情况或条件。状态可以被看作是系统内部变量的一种表示。
输入：也称为事件或信号，是触发状态间转移的条件。
状态转移规则：定义了根据输入从一个状态转移到另一个状态的规则。
输出：与每个状态关联的动作或结果，但并非所有有限状态机都有输出，有时仅存在状态转移。
初始状态：系统开始时所处的状态。
工作原理可以概括为：系统从初始状态开始，根据接收到的输入（事件）和当前的状态，按照状态转移规则转移到下一个状态。在执行过程中，状态机会不断根据输入和当前状态更新其状态。

## **APP实现功能**

本APP基于状态机思想，实现lwm2m方式接入onenet并定时上传数据，包括以下子功能：

1. 有限状态机定义，及状态转移表枚举；
2. OneNET登录、保活、上传数据等。

## **APP执行流程**

1、创建app线程，并在线程内等待驻网。
2、驻网成功后，状态机启动并等待接收输入事件；定时器启动，并定时为状态机反馈事件。
osMessageQueuePut(s_msg_q, &msg, 0, 0);
3、app线程接收事件，并根据状态转移表执行对应的action。
static inline int __onenet_machine_run(cm_onenet_state_machine_t *machine, cm_onenet_event_e evt)
{
    int i = 0;
    cm_onenet_state_trans_t *trans = NULL;

    for(i = 0; i < machine->machine_sz; i++)
    {
        if(machine->state == machine->machine_tbl[i].cur_state
          && evt == machine->machine_tbl[i].event)  //检索当前状态节点，并匹配迁移路线
        {
            trans = (cm_onenet_state_trans_t*)&machine->machine_tbl[i];
        }
    }

    if(trans == NULL)
    {
        cm_log_printf(0, "state machine step err,cur_state:%d,evt:%d", machine->state, evt);
        return -1;
    }

    int pre_state = machine->state;
    machine->state = trans->action(trans);

    if(machine->state == CM_ONENET_STATE_CONNECTED) //连接之后直接切换为IDLE状态
    {
        machine->state = CM_ONENET_STATE_IDLE;
    }

    cm_log_printf(0, "state machine step %d -> %d", pre_state, machine->state);
	return 0;
}

## **APP使用说明**

- 支持的模组（子）型号：ML307A-DSLN/ML307A-DCLN/ML307R-DC/ML307C-DC-CN
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0/ML307C OpenCPU SDK 1.0.0版本及其后续版本
- 是否需要外设支撑：
- 使用注意事项：

  
## **FAQ**

- 无

## **版本更新说明**

### **1.0.1版本**
- 发布时间：2024/12/24 11:32
- 修改记录：
  1. 新增支持的模组（子）型号以及支持的SDK版本

### **1.0.0版本**

- 发布时间：2024/10/23 18:20
- 修改记录：
  1. 初版

---
