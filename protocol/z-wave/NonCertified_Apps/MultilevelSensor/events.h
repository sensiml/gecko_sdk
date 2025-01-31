/**
 * @file
 *
 * Definitions of events for Multilevel Sensor Certified App.
 *
 * @copyright 2019 Silicon Laboratories Inc.
 */
#ifndef _APPS_MULTILEVELSENSOR_EVENTS_H_
#define _APPS_MULTILEVELSENSOR_EVENTS_H_

/**
 * Defines events for the application.
 *
 * These events are not referred to anywhere else than in the application. Hence, they can be
 * altered to suit the application flow.
 *
 * The events are located in a separate file to make it possible to include them in other
 * application files. An example could be a peripheral driver that enqueues an event when something
 * specific happens, e.g. on motion detection.
 */
typedef enum EVENT_APP_MULTILEVELSENSOR
{
  EVENT_EMPTY = DEFINE_EVENT_APP_NBR,
  EVENT_APP_INIT,
  EVENT_APP_FLUSHMEM_READY,
  EVENT_APP_NEXT_EVENT_JOB,
  EVENT_APP_FINISH_EVENT_JOB,
  EVENT_APP_SEND_BATTERY_LEVEL_REPORT,
  EVENT_APP_BASIC_STOP_JOB,
  EVENT_APP_BASIC_START_JOB,
  EVENT_APP_NOTIFICATION_START_JOB,
  EVENT_APP_NOTIFICATION_STOP_JOB,
  EVENT_APP_START_TIMER_EVENTJOB_STOP,
  EVENT_APP_SMARTSTART_IN_PROGRESS,
  EVENT_APP_LEARN_IN_PROGRESS,
  /*
   * User-Task related events towards the Main App!
   */
  EVENT_APP_USERTASK_DATA_ACQUISITION_READY,
  EVENT_APP_USERTASK_DATA_ACQUISITION_FINISHED,
}
EVENT_APP;

#endif /* _APPS_MULTILEVELSENSOR_EVENTS_H_ */
