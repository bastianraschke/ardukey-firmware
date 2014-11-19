typedef struct
{
  /* Unique (secret) ID. */
  uint8_t uid[YUBIKEY_UID_SIZE];
  /* Session counter (incremented by 1 at each startup).  High bit
     indicates whether caps-lock triggered the token. */
  uint16_t ctr;
  /* Timestamp incremented by approx 8Hz (low part). */
  uint16_t tstpl;
  /* Timestamp (high part). */
  uint8_t tstph;
  /* Number of times used within session + activation flags. */
  uint8_t use;
  /* Pseudo-random value. */
  uint16_t rnd;
  /* CRC16 value of all fields. */
  uint16_t crc;
} yubikey_token_st;


tok.use = ctx->usage;
tok.ctr = ctx->counter;

// time stamp
tok.tstpl = (uint16_t) ctx->timestamp & 0xffff;
tok.tstph = (uint8_t) (ctx->timestamp >> 16) & 0xff;

tok.rnd = random (0x10000);
tok.crc = ~yubikey_crc16 ((const uint8_t *) &tok, sizeof (tok) - sizeof (tok.crc));
