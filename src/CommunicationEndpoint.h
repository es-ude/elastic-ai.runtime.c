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
void CommunicationEndpointSetDomain(char *domain);

/*! Returns the Domain of the Broker
 *
 * @return Domain of the Broker
 */
char *CommunicationEndpointGetDomain();

/*!
 *
 * @param deviceIdentifier Unique identifier of the device in the MQTT network
 */
void CommunicationEndpointSetDeviceId(char *deviceIdentifier);

/*!
 *
 * @return Unique identifier of the device in the MQTT network
 */
char *CommunicationEndpointGetDeviceId();

/*! Publishes the posting_t, including the Domain and Device identification in the topic
 *
 * @param posting_t posting_t to be published
 */
void CommunicationEndpointPublish(posting_t posting);

/*! Publishes the posting_t, adding the Domain in the topic
 *
 * @param posting_t posting_t to be published
 */
void CommunicationEndpointPublishRemote(posting_t posting);

/*! Publishes the posting_t as it is
 *
 * @param posting_t posting_t to be published
 */
void CommunicationEndpointPublishRaw(posting_t posting);

/*! Subscribes to the topic adding the Domain and device identification
 *
 * @param topic Topic to subscribe to Topic to CommunicationEndpointSubscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void CommunicationEndpointSubscribe(char *topic, subscriber_t subscriber);

/*! Subscribes to the topic adding the Domain
 *
 * @param topic Topic to CommunicationEndpointSubscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void CommunicationEndpointSubscribeRemote(char *topic, subscriber_t subscriber);

/*! Subscribes to the topic as it is
 *
 * @param topic Topic to CommunicationEndpointSubscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void CommunicationEndpointSubscribeRaw(char *topic, subscriber_t subscriber);

/*! Unsubscribes to the topic adding the Domain and device identification
 *
 * @param topic Topic to CommunicationEndpointSubscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void CommunicationEndpointUnsubscribe(char *topic, subscriber_t subscriber);

/*! Unsubscribes to the topic adding the Domain
 *
 * @param topic Topic to CommunicationEndpointSubscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void CommunicationEndpointUnsubscribeRemote(char *topic, subscriber_t subscriber);

/*! Unsubscribes to the topic as it is
 *
 * @param topic Topic to CommunicationEndpointSubscribe to
 * @param subscriber Function to be called when message is published to topic
 */
void CommunicationEndpointUnsubscribeRaw(char *topic, subscriber_t subscriber);

#endif /* ELASTIC_AI_RUNTIME_C_COMMUNICATION_ENDPOINT_H */
