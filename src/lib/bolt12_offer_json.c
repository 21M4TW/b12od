#include <inttypes.h>

#include <b12od/bolt12_offer_json.h>
#include <b12od/bolt12_json_tlv.h>

// Important: Vector elements must be sorted
static struct bolt12_json_vector_element const _b12j_vector[] = {
  {TYPE_OFFER_CHAINS, bolt12_json_add_offer_chains},
  {TYPE_OFFER_METADATA, bolt12_json_add_offer_metadata},
  {TYPE_OFFER_CURRENCY, bolt12_json_add_offer_currency},
  {TYPE_OFFER_AMOUNT, bolt12_json_add_offer_amount},
  {TYPE_OFFER_DESCRIPTION, bolt12_json_add_offer_description},
  {TYPE_OFFER_FEATURES, bolt12_json_add_offer_features},
  {TYPE_OFFER_ABSOLUTE_EXPIRY, bolt12_json_add_offer_absolute_expiry},
  {TYPE_OFFER_PATHS, bolt12_json_add_offer_paths},
  {TYPE_OFFER_ISSUER, bolt12_json_add_offer_issuer},
  {TYPE_OFFER_QUANTITY_MAX, bolt12_json_add_offer_quantity_max},
  {TYPE_OFFER_ISSUER_ID, bolt12_json_add_offer_issuer_id},
  {0, bolt12_json_add_unknown_tlv_field}
};
static const size_t _b12j_vector_length = sizeof(_b12j_vector) / sizeof(struct bolt12_json_vector_element) - 1;

struct bolt12_json* bolt12_offer_json_new()
{
  struct bolt12_json* const b12j = bolt12_json_new();

  if(!b12j) return NULL;
  b12j->b12 = malloc(sizeof(struct bolt12_offer));

  if(!b12j->b12) {
    bolt12_json_delete(b12j);
    return NULL;
  }
  init_bolt12_offer(b12j->b12);
  b12j->vector = _b12j_vector;
  b12j->vector_length = _b12j_vector_length;
  return b12j;
}

void bolt12_offer_json_delete(struct bolt12_json* const b12j)
{
  bolt12_free_records(b12j->b12);
  free(b12j->b12);
  bolt12_json_delete(b12j);
}

void bolt12_offer_json_shrink_to_fit(struct bolt12_json* const b12j)
{
  json_shrink_to_fit(&b12j->jctx);
}

size_t bolt12_offer_json_get_size(struct bolt12_json* const b12j)
{
  return b12j->jctx.bb.size - 1;
}
