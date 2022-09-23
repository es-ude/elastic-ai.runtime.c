#ifndef ELASTIC_AI_RUNTIME_C_COMMUNICATION_ENDPOINT_H
#define ELASTIC_AI_RUNTIME_C_COMMUNICATION_ENDPOINT_H

#include "Posting.h"
#include "Subscriber.h"

/*!
 * A MQTT-Broker implementation needs to implements these functions.
 */

/*! Sets the Domain of the Broker
 *
 * @param domain Domain of the Broker
 */
void communicationEndpointSetDomain(char *domain);

/*! Returns the Domain of the Broker
 *
 * @return Domain of the Broker
 */
char *communicationEndpointGetDomain();

/*!
 *
 * @param deviceIdentifier Unique identifier of the device in the MQTT network
 */
void communicationEndpointSetDeviceId(char *deviceIdentifier);

/*!
 *
 * @return Unique identifier of the device in the MQTT network
 */
char *communicationEndpointGetDeviceId();

/*! Publishes the Posting, including the Domain and Device identification in the topic
 *
 * @param posting Posting to be published
 */
void communicationEndpointPublish(posting_t posting);

/*! Publishes the Posting, adding the Domain in the topic
 *
 * @param posting Posting to be published
 */
void communicationEndpointPublishRemote(posting_t posting);

/*! Publishes the Posting as it is
 *
 * @param posting Posting to be published
 */
void communicationEndpointPublishRaw(posting_t posting);

/*! Subscribes to the topic adding the Domain and device identification
 *
 * @param topic Topic to subscribe
 * @param subscriber Function to be called when message is published to topic
 */
void communicationEndpointSubscribe(char *topic, subscriber_t subscriber);

/*! Subscribes to the topic adding the Domain
 *
 * @param topic Topic to subscribe
 * @param subscriber Function to be called when message is published to topic
 */
void communicationEndpointSubscribeRemote(char *topic, subscriber_t subscriber);

/*! Subscribes to the topic as it is
 *
 * @param topic Topic to subscribe
 * @param subscriber Function to be called when message is published to topic
 */
void communicationEndpointSubscribeRaw(char *topic, subscriber_t subscriber);

/*! Unsubscribes to the topic adding the Domain and device identification
 *
 * @param topic Topic to unsubscribe
 * @param subscriber Function to be called when message is published to topic
 */
void communicationEndpointUnsubscribe(char *topic, subscriber_t subscriber);

/*! Unsubscribes to the topic adding the Domain
 *
 * @param topic Topic to unsubscribe
 * @param subscriber Function to be called when message is published to topic
 */
void communicationEndpointUnsubscribeRemote(char *topic, subscriber_t subscriber);

/*! Unsubscribes to the topic as it is
 *
 * @param topic Topic to unsubscribe
 * @param subscriber Function to be called when message is published to topic
 */
void communicationEndpointUnsubscribeRaw(char *topic, subscriber_t subscriber);

#endif /* ELASTIC_AI_RUNTIME_C_COMMUNICATION_ENDPOINT_H */
