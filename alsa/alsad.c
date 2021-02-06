#include <linux/init.h>   // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/device.h> // Header to support the kernel Driver Model
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/fs.h>     // Header for the Linux file system support
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>

struct alsad_device{
  struct snd_card *card;
  struct snd_pcm  *pcm;
};

static int index = -1;
static char *id = NULL;

static struct alsad_device *alsad = NULL;

static struct snd_pcm_hardware alsad_capture_hw = {
  .info = (SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_BLOCK_TRANSFER | SNDRV_PCM_INFO_INTERLEAVED | SNDRV_PCM_INFO_MMAP_VALID),
  .formats = SNDRV_PCM_FMTBIT_U16_LE,
  .rates = SNDRV_PCM_RATE_48000,
  .rate_min = SNDRV_PCM_RATE_48000,
  .rate_max = SNDRV_PCM_RATE_48000,
  .channels_min = 1,
  .channels_max = 1,
  .buffer_bytes_max = 32768,
};

static int alsad_capture_open(struct snd_pcm_substream *substream)
{
  struct snd_pcm_runtime *runtime = substream->runtime;

  printk(KERN_ALERT "alsad_capture_open called\n");

  runtime->hw = alsad_capture_hw;

  return 0;
}

static int alsad_capture_close(struct snd_pcm_substream *substream)
{
  printk(KERN_ALERT "alsad_capture_close called\n");
  return 0;
}

static int alsad_pcm_hw_params(struct snd_pcm_substream *substream,struct snd_pcm_hw_params *hw_params)
{
  return snd_pcm_lib_malloc_pages(substream,params_buffer_bytes(hw_params));
}

static int alsad_pcm_hw_free(struct snd_pcm_substream *substream)
{
  return snd_pcm_lib_free_pages(substream);
}

static int alsad_capture_prepare(struct snd_pcm_substream *substream)
{
  printk(KERN_ALERT "alsad_capture_prepare called\n");
  return 0;
}

static int alsad_capture_trigger(struct snd_pcm_substream *substream, int cmd)
{
  printk(KERN_ALERT "alsad_capture_trigger called\n");
  return 0;
}

static struct snd_pcm_ops alsad_capture_ops = {
  .open = alsad_capture_open,
  .close = alsad_capture_close,
  .ioctl = snd_pcm_lib_ioctl,
  .hw_params = alsad_pcm_hw_params,
  .hw_free = alsad_pcm_hw_free,
  .prepare = alsad_capture_prepare,
  .trigger = alsad_capture_trigger,
};

static int alsad_dev_register(struct snd_device *dev)
{
  printk(KERN_ALERT "alsad_dev_register called\n");
  return 0;
}

static int alsad_dev_disconnect(struct snd_device *dev)
{
  printk(KERN_ALERT "alsad_dev_disconnect called\n");
  return 0;
}

static int alsad_dev_free(struct snd_device *dev)
{
  printk(KERN_ALERT "alsad_dev_free called\n");
  return 0;
}

static struct snd_device_ops ops = {
  .dev_register = alsad_dev_register,
  .dev_disconnect = alsad_dev_disconnect,
  .dev_free = alsad_dev_free
};

static int __init alsad_init(void)
{
  int err = 0;

  alsad = kzalloc(sizeof(struct alsad_device),GFP_KERNEL);

  if(!alsad)
    return -ENOMEM;

  err = snd_card_new(0,index,id,THIS_MODULE,0,&alsad->card);

  if(err < 0)
    return err;

  snd_device_new(alsad->card,SNDRV_DEV_LOWLEVEL,alsad,&ops);

  err = snd_pcm_new(alsad->card,"alsad_pcm",0,0,1,&alsad->pcm);

  if(err < 0)
  {
    snd_card_free(alsad->card);
    return err;
  }

  strcpy(alsad->card->driver,"alsad_pcm");

  alsad->pcm->private_data = alsad;

  snd_pcm_set_ops(alsad->pcm,SNDRV_PCM_STREAM_CAPTURE,&alsad_capture_ops);

  snd_pcm_lib_preallocate_pages_for_all(alsad->pcm,SNDRV_DMA_TYPE_CONTINUOUS,snd_dma_continuous_data(GFP_KERNEL),64*1024,64*1024);

  err = snd_card_register(alsad->card);
  if(err < 0)
  {
    return err;
  }

  return 0;
}

static void __exit alsad_exit(void)
{
  snd_card_free(alsad->card);
  kfree(alsad);
}

module_init(alsad_init);
module_exit(alsad_exit);

MODULE_LICENSE("GPL");
