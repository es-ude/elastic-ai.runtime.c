#ifndef ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
#define ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H

#include "posting.h"
#include "subscriber.h"

/*!
 * A MQTT-Broker implementation needs to implements these functions.
 */

/*! Sets the Domain of the Broker
 *
 * @param domain Domain of the Broker
 */
void setDomain(char *domain);

/*! Returns the Domain of the Broker
 *
 * @return Domain of the Broker
 */
char *getDomain();

/*!
 *
 * @param deviceIdentifier Unique identifier of the device in the MQTT network
 */
void setDeviceID(char *deviceIdentifier);

/*!
 *
 * @return Unique identifier of the device in the MQTT network
 */
char *getDeviceID();

/*! Publishes the posting, including the Domain and Device identification in the topic
 *
 * @param posting Posting to be published
 */
void publish(Posting posting);

/*! Publishes the Posting, adding the Domain in the topic
 *
 * @param posting Posting to be published
 */
void publishRemote(Posting posting);

/*! Publishes the Posting as it is
 *
 * @param posting Posting to be published
 */
void publishRaw(Posting posting);

/*! Subscribes to the topic adding the Domain and device identification
 *
 * @param topic Topic to subscribe to Topic to subscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void subscribe(char *topic, Subscriber subscriber);

/*! Subscribes to the topic adding the Domain
 *
 * @param topic Topic to subscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void subscribeRemote(char *topic, Subscriber subscriber);

/*! Subscribes to the topic as it is
 *
 * @param topic Topic to subscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void subscribeRaw(char *topic, Subscriber subscriber);

/*! Unsubscribes to the topic adding the Domain and device identification
 *
 * @param topic Topic to subscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void unsubscribe(char *topic, Subscriber subscriber);

/*! Unsubscribes to the topic adding the Domain
 *
 * @param topic Topic to subscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void unsubscribeRemote(char *topic, Subscriber subscriber);

/*! Unsubscribes to the topic as it is
 *
 * @param topic Topic to subscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void unsubscribeRaw(char *topic, Subscriber subscriber);

#endif // ELASTIC_AI_RUNTIME_C_COMMUNICATIONENDPOINT_H
