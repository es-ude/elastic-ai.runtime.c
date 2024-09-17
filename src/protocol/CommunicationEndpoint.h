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

void communicationEndpointSetBrokerDomain(char *brokerDomain);

/*! Returns the Domain of the Broker
 *
 * @return Domain of the Broker
 */

char *communicationEndpointGetBrokerDomain();

/*! Sets the identifier of the clients and the user identifier with the corresponding password
 *
 * @param clientId Identifier of the Client
 * @param userId User Identifier
 * @param password Password correlating with the user identifier
 */

uint8_t communicationEndpointSetUserConfiguration(char *clientId, char *userId, char *password);

/*! Returns the identifier of the client
 *
 * @return Identifier of the Client
 */
char *communicationEndpointGetClientId();

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
void communicationEndpointUnsubscribeRaw(char *topic,
                                         __attribute__((unused)) subscriber_t subscriber);

#endif /* ELASTIC_AI_RUNTIME_C_COMMUNICATION_ENDPOINT_H */
