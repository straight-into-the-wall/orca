#include <stdio.h>

#include "discord.h"

void on_ready(struct discord* client, const struct discord_user* bot)
{
  log_info("Logged in as %s!", bot->username);
}

void log_msg(const struct discord_message* msg)
{
  log_info("MSG received on %lld from %s content: %s |", msg->channel_id,
           msg->author->username, msg->content);
}

void on_bum(struct discord* client, const struct discord_user* bot,
            const struct discord_message* msg)
{
  if (msg->author->bot) return; // ignore bots

  struct discord_create_message_params params = { .content = "crash" };
  discord_create_message(client, msg->channel_id, &params, NULL);
  log_msg(msg);
}

void on_crash(struct discord* client, const struct discord_user* bot,
              const struct discord_message* msg)
{
  if (msg->author->bot) return; // ignore bots

  struct discord_create_message_params params = { .content = "bum" };
  discord_create_message(client, msg->channel_id, &params, NULL);
  log_msg(msg);
}

int main()
{
  struct discord* client = discord_config_init("./mybot_config.json");

  discord_set_on_ready(client, &on_ready);
  discord_set_on_command(client, "bum", &on_bum);
  discord_set_on_command(client, "crash", &on_crash);

  discord_run(client);

  discord_cleanup(client);
  return 0;
}
