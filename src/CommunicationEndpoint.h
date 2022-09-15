#ifndef ELASTIC_AI_RUNTIME_C_COMMUNICATION_ENDPOINT_H
#define ELASTIC_AI_RUNTIME_C_COMMUNICATION_ENDPOINT_H

#include "Posting.h"
#include "Subscriber.h"
#include <stdint.h>

/*!
 * A MQTT-Broker implementation needs to implements these functions.
 */

/*! Sets the Domain of the Broker
 *
 * @param brokerDomain Domain of the Broker
 */
void CommunicationEndpointSetBrokerDomain(char *brokerDomain);

/*! Returns the Domain of the Broker
 *
 * @return Domain of the Broker
 */
char *CommunicationEndpointGetBrokerDomain();

/*! Sets the identifier of the clients and the user identifier with the corresponding password
 *
 * @param clientId Identifier of the Client
 * @param userId User Identifier
 * @param password Password correlating with the user identifier
 */
uint8_t CommunicationEndpointSetUserConfiguration(char *clientId, char *userId, char *password);

/*! Returns the identifier of the client
 *
 * @return Identifier of the Client
 */
char *CommunicationEndpointGetClientId();

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
